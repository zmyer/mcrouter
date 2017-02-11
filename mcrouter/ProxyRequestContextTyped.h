/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "mcrouter/ProxyRequestContext.h"
#include "mcrouter/ProxyRequestLogger.h"
#include "mcrouter/lib/RequestLoggerContext.h"
#include "mcrouter/lib/carbon/NoopAdditionalLogger.h"

namespace facebook {
namespace memcache {
namespace mcrouter {

template <class RouterInfo>
class ProxyConfig;
template <class RouterInfo>
class Proxy;
namespace detail {

template <class T>
struct Void {
  using type = void;
};

template <class RouterInfo, class U = void>
struct RouterAdditionalLogger {
  using type = carbon::NoopAdditionalLogger;
};
template <class RouterInfo>
struct RouterAdditionalLogger<
    RouterInfo,
    typename Void<typename RouterInfo::AdditionalLogger>::type> {
  using type = typename RouterInfo::AdditionalLogger;
};

} // detail

template <class RouterInfo>
class ProxyRequestContextWithInfo : public ProxyRequestContext {
 public:
  /**
   * A request with this context will not be sent/logged anywhere.
   *
   * @param clientCallback  If non-nullptr, called by DestinationRoute when
   *   the request would normally be sent to destination;
   *   also in traverse() of DestinationRoute.
   * @param shardSplitCallback  If non-nullptr, called by ShardSplitRoute
   *   in traverse() with itself as the argument.
   */
  static std::shared_ptr<ProxyRequestContextWithInfo<RouterInfo>>
  createRecording(
      Proxy<RouterInfo>& proxy,
      ClientCallback clientCallback,
      ShardSplitCallback shardSplitCallback = nullptr) {
    return std::shared_ptr<ProxyRequestContextWithInfo<RouterInfo>>(
        new ProxyRequestContextWithInfo<RouterInfo>(
            Recording,
            proxy,
            std::move(clientCallback),
            std::move(shardSplitCallback)));
  }

  /**
   * Same as createRecording(), but also notifies the baton
   * when this context is destroyed (i.e. all requests referencing it
   * finish executing).
   */
  static std::shared_ptr<ProxyRequestContextWithInfo<RouterInfo>>
  createRecordingNotify(
      Proxy<RouterInfo>& proxy,
      folly::fibers::Baton& baton,
      ClientCallback clientCallback,
      ShardSplitCallback shardSplitCallback = nullptr) {
    return std::shared_ptr<ProxyRequestContextWithInfo<RouterInfo>>(
        new ProxyRequestContextWithInfo<RouterInfo>(
            Recording,
            proxy,
            std::move(clientCallback),
            std::move(shardSplitCallback)),
        [&baton](ProxyRequestContext* ctx) {
          delete ctx;
          baton.post();
        });
  }

  ~ProxyRequestContextWithInfo() override {
    if (reqComplete_) {
      fiber_local<RouterInfo>::runWithoutLocals(
          [this]() { reqComplete_(*this); });
    }
  }

  /**
   * Called once a reply is received to record a stats sample if required.
   */
  template <class Request>
  void onReplyReceived(
      const std::string& poolName,
      const AccessPoint& ap,
      folly::StringPiece strippedRoutingPrefix,
      const Request& request,
      const ReplyT<Request>& reply,
      RequestClass requestClass,
      const int64_t startTimeUs,
      const int64_t endTimeUs,
      const ReplyStatsContext replyStatsContext) {
    if (recording()) {
      return;
    }

    RequestLoggerContext loggerContext(
        poolName,
        ap,
        strippedRoutingPrefix,
        requestClass,
        startTimeUs,
        endTimeUs,
        reply.result(),
        replyStatsContext);
    assert(logger_.hasValue());
    logger_->template log<Request>(loggerContext);
    assert(additionalLogger_.hasValue());
    additionalLogger_->log(request, reply, loggerContext);
  }

 private:
  using AdditionalLogger =
      typename detail::RouterAdditionalLogger<RouterInfo>::type;

 protected:
  ProxyRequestContextWithInfo(
      Proxy<RouterInfo>& pr,
      ProxyRequestPriority priority__)
      : ProxyRequestContext(pr, priority__),
        proxy_(pr),
        logger_(ProxyRequestLogger<RouterInfo>(pr)),
        additionalLogger_(AdditionalLogger(&pr)) {}

  Proxy<RouterInfo>& proxy_;

 private:
  ProxyRequestContextWithInfo(
      RecordingT,
      Proxy<RouterInfo>& pr,
      ClientCallback clientCallback,
      ShardSplitCallback shardSplitCallback = nullptr)
      : ProxyRequestContext(
            Recording,
            pr,
            std::move(clientCallback),
            std::move(shardSplitCallback)),
        proxy_(pr) {}

  folly::Optional<ProxyRequestLogger<RouterInfo>> logger_;
  folly::Optional<AdditionalLogger> additionalLogger_;
};

template <class RouterInfo, class Request>
class ProxyRequestContextTyped
    : public ProxyRequestContextWithInfo<RouterInfo> {
 public:
  using Type = ProxyRequestContextTyped<RouterInfo, Request>;
  /**
   * Sends the reply for this proxy request.
   * @param newReply the message that we are sending out as the reply
   *   for the request we are currently handling
   */
  void sendReply(ReplyT<Request>&& reply);

  /**
   * DEPRECATED. Convenience method, that constructs reply and calls
   * non-template method.
   *
   * WARNING: This function can be dangerous with new typed requests.
   * For typed requests,
   *   ctx->sendReply(mc_res_local_error, "Error message")
   * does the right thing, while
   *   ctx->sendReply(mc_res_found, "value")
   * does the wrong thing.
   */
  template <class... Args>
  void sendReply(Args&&... args) {
    sendReply(ReplyT<Request>(std::forward<Args>(args)...));
  }

  void startProcessing() override final;

  const ProxyConfig<RouterInfo>& proxyConfig() const {
    assert(!this->recording());
    return *config_;
  }

  ProxyRoute<RouterInfo>& proxyRoute() const {
    assert(!this->recording());
    return config_->proxyRoute();
  }

  /**
   * Internally converts the context into one ready to route.
   * Config pointer is saved to keep the config alive, and
   * ownership is changed to shared so that all subrequests
   * keep track of this context.
   */
  static std::shared_ptr<Type> process(
      std::unique_ptr<Type> preq,
      std::shared_ptr<const ProxyConfig<RouterInfo>> config);

 protected:
  ProxyRequestContextTyped(
      Proxy<RouterInfo>& pr,
      const Request& req,
      ProxyRequestPriority priority__)
      : ProxyRequestContextWithInfo<RouterInfo>(pr, priority__),
        req_(&req) {}

  std::shared_ptr<const ProxyConfig<RouterInfo>> config_;

  // It's guaranteed to point to an existing request until we call user callback
  // (i.e. replied_ changes to true), after that it's nullptr.
  const Request* req_;

  virtual void sendReplyImpl(ReplyT<Request>&& reply) = 0;

 private:
  friend class Proxy<RouterInfo>;
};

/**
 * Creates a new proxy request context
 */
template <class RouterInfo, class Request, class F>
std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>>
createProxyRequestContext(
    Proxy<RouterInfo>& pr,
    const Request& req,
    F&& f,
    ProxyRequestPriority priority = ProxyRequestPriority::kCritical);

} // mcrouter
} // memcache
} // facebook

#include "ProxyRequestContextTyped-inl.h"
