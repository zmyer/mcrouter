/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <folly/fibers/EventBaseLoopController.h>

#include "mcrouter/McrouterFiberContext.h"
#include "mcrouter/ProxyRequestContextTyped.h"
#include "mcrouter/lib/MessageQueue.h"
#include "mcrouter/lib/carbon/RoutingGroups.h"
#include "mcrouter/lib/carbon/Stats.h"
#include "mcrouter/lib/network/gen/Memcache.h"
#include "mcrouter/options.h"
#include "mcrouter/routes/ProxyRoute.h"
#include "mcrouter/stats.h"

namespace facebook {
namespace memcache {
namespace mcrouter {

template <class RouterInfo>
class ProxyConfig;

namespace detail {

// TODO(@aap): Make ServceInfo work with something other than
//             MemcacheRouterInfo/McGetRequest
bool processGetServiceInfoRequest(
    const McGetRequest& req,
    std::shared_ptr<ProxyRequestContextTyped<McrouterRouterInfo, McGetRequest>>&
        ctx);

template <class RouterInfo, class Request>
bool processGetServiceInfoRequest(
    const Request&,
    std::shared_ptr<ProxyRequestContextTyped<RouterInfo, Request>>&) {
  return false;
}

template <class RouterInfo, class GetRequest>
bool processGetServiceInfoRequestImpl(
    const GetRequest& req,
    std::shared_ptr<ProxyRequestContextTyped<RouterInfo, GetRequest>>& ctx,
    carbon::GetLikeT<GetRequest> = 0);

} // detail

template <class RouterInfo>
template <class Request>
Proxy<RouterInfo>::WaitingRequest<Request>::WaitingRequest(
    const Request& req,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>> ctx)
    : req_(req), ctx_(std::move(ctx)) {}

template <class RouterInfo>
template <class Request>
void Proxy<RouterInfo>::WaitingRequest<Request>::process(
    Proxy<RouterInfo>* proxy) {
  // timePushedOnQueue_ is nonnegative only if waiting-requests-timeout is
  // enabled
  if (timePushedOnQueue_ >= 0) {
    const auto durationInQueueUs = nowUs() - timePushedOnQueue_;

    if (durationInQueueUs >
        1000 * static_cast<int64_t>(
                   proxy->getRouterOptions().waiting_request_timeout_ms)) {
      ctx_->sendReply(mc_res_busy);
      return;
    }
  }

  proxy->processRequest(req_, std::move(ctx_));
}

template <class RouterInfo>
template <class Request>
typename std::enable_if<
    ListContains<typename RouterInfo::RoutableRequests, Request>::value,
    void>::type
Proxy<RouterInfo>::routeHandlesProcessRequest(
    const Request& req,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>> uctx) {
  requestStats_.template bump<Request>(carbon::RouterStatTypes::Incoming);

  auto sharedCtx = ProxyRequestContextTyped<RouterInfo, Request>::process(
      std::move(uctx), getConfigUnsafe());

  if (detail::processGetServiceInfoRequest(req, sharedCtx)) {
    return;
  }

  auto funcCtx = sharedCtx;

  fiberManager().addTaskFinally(
      [&req, ctx = std::move(funcCtx) ]() mutable {
        try {
          auto& proute = ctx->proxyRoute();
          fiber_local<RouterInfo>::setSharedCtx(std::move(ctx));
          return proute.route(req);
        } catch (const std::exception& e) {
          auto err = folly::sformat(
              "Error routing request of type {}!"
              " Exception: {}",
              typeid(Request).name(),
              e.what());
          ReplyT<Request> reply(mc_res_local_error);
          carbon::setMessageIfPresent(reply, std::move(err));
          return reply;
        }
      },
      [ctx = std::move(sharedCtx)](folly::Try<ReplyT<Request>> && reply) {
        ctx->sendReply(std::move(*reply));
      });
}

template <class RouterInfo>
template <class Request>
typename std::enable_if<
    !ListContains<typename RouterInfo::RoutableRequests, Request>::value,
    void>::type
Proxy<RouterInfo>::routeHandlesProcessRequest(
    const Request&,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>> uctx) {
  ReplyT<Request> reply(mc_res_local_error);
  carbon::setMessageIfPresent(
      reply,
      folly::sformat(
          "Couldn't route request of type {} "
          "because the operation is not supported by RouteHandles "
          "library!",
          typeid(Request).name()));
  uctx->sendReply(std::move(reply));
}

template <class RouterInfo>
template <class Request>
void Proxy<RouterInfo>::processRequest(
    const Request& req,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>> ctx) {
  assert(!ctx->isProcessing());
  ctx->markAsProcessing();
  ++numRequestsProcessing_;
  stats().increment(proxy_reqs_processing_stat);

  routeHandlesProcessRequest(req, std::move(ctx));

  stats().increment(request_sent_stat);
  stats().increment(request_sent_count_stat);
}

template <class RouterInfo>
template <class Request>
void Proxy<RouterInfo>::dispatchRequest(
    const Request& req,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, Request>> ctx) {
  if (rateLimited(ctx->priority(), req)) {
    if (getRouterOptions().proxy_max_throttled_requests > 0 &&
        numRequestsWaiting_ >=
            getRouterOptions().proxy_max_throttled_requests) {
      ctx->sendReply(mc_res_busy);
      return;
    }
    auto& queue = waitingRequests_[static_cast<int>(ctx->priority())];
    auto w = folly::make_unique<WaitingRequest<Request>>(req, std::move(ctx));
    // Only enable timeout on waitingRequests_ queue when queue throttling is
    // enabled
    if (getRouterOptions().proxy_max_inflight_requests > 0 &&
        getRouterOptions().proxy_max_throttled_requests > 0 &&
        getRouterOptions().waiting_request_timeout_ms > 0) {
      w->setTimePushedOnQueue(nowUs());
    }
    queue.pushBack(std::move(w));
    ++numRequestsWaiting_;
    stats().increment(proxy_reqs_waiting_stat);
  } else {
    processRequest(req, std::move(ctx));
  }
}

template <class RouterInfo>
Proxy<RouterInfo>::Proxy(
    CarbonRouterInstanceBase& rtr,
    size_t id,
    folly::EventBase& evb)
    : ProxyBase(rtr, id, evb, RouterInfo()) {
  messageQueue_ = folly::make_unique<MessageQueue<ProxyMessage>>(
      router().opts().client_queue_size,
      [this](ProxyMessage&& message) {
        this->messageReady(message.type, message.data);
      },
      router().opts().client_queue_no_notify_rate,
      router().opts().client_queue_wait_threshold_us,
      &nowUs,
      [this]() { stats().incrementSafe(client_queue_notifications_stat); });
}

template <class RouterInfo>
typename Proxy<RouterInfo>::Pointer Proxy<RouterInfo>::createProxy(
    CarbonRouterInstanceBase& router,
    folly::EventBase& eventBase,
    size_t id) {
  /* This hack is needed to make sure Proxy stays alive
     until at least event base managed to run the callback below */
  auto proxy = std::shared_ptr<Proxy>(new Proxy(router, id, eventBase));
  proxy->self_ = proxy;

  eventBase.runInEventBaseThread([proxy, &eventBase]() {
    proxy->messageQueue_->attachEventBase(eventBase);

    dynamic_cast<folly::fibers::EventBaseLoopController&>(
        proxy->fiberManager().loopController())
        .attachEventBase(eventBase);

    std::chrono::milliseconds connectionResetInterval{
        proxy->router().opts().reset_inactive_connection_interval};

    if (connectionResetInterval.count() > 0) {
      proxy->destinationMap()->setResetTimer(connectionResetInterval);
    }
  });

  return Pointer(proxy.get());
}

template <class RouterInfo>
std::shared_ptr<ProxyConfig<RouterInfo>> Proxy<RouterInfo>::getConfigUnsafe()
    const {
  std::lock_guard<SFRReadLock> lg(const_cast<SFRLock&>(configLock_).readLock());
  return config_;
}

template <class RouterInfo>
std::pair<std::unique_lock<SFRReadLock>, ProxyConfig<RouterInfo>&>
Proxy<RouterInfo>::getConfigLocked() const {
  std::unique_lock<SFRReadLock> lock(
      const_cast<SFRLock&>(configLock_).readLock());
  /* make_pair strips the reference, so construct directly */
  return std::pair<std::unique_lock<SFRReadLock>, ProxyConfig<RouterInfo>&>(
      std::move(lock), *config_);
}

template <class RouterInfo>
std::shared_ptr<ProxyConfig<RouterInfo>> Proxy<RouterInfo>::swapConfig(
    std::shared_ptr<ProxyConfig<RouterInfo>> newConfig) {
  std::lock_guard<SFRWriteLock> lg(configLock_.writeLock());
  auto old = std::move(config_);
  config_ = std::move(newConfig);
  return old;
}

/** drain and delete proxy object */
template <class RouterInfo>
Proxy<RouterInfo>::~Proxy() {
  destinationMap_.reset();

  beingDestroyed_ = true;

  if (messageQueue_) {
    messageQueue_->drain();
  }
}

template <class RouterInfo>
void Proxy<RouterInfo>::sendMessage(ProxyMessage::Type t, void* data) noexcept {
  CHECK(messageQueue_.get());
  messageQueue_->blockingWrite(t, data);
}

template <class RouterInfo>
size_t Proxy<RouterInfo>::queueNotifyPeriod() const {
  if (messageQueue_) {
    return messageQueue_->currentNotifyPeriod();
  }
  return 0;
}

template <class RouterInfo>
void Proxy<RouterInfo>::messageReady(ProxyMessage::Type t, void* data) {
  switch (t) {
    case ProxyMessage::Type::REQUEST: {
      auto preq = reinterpret_cast<ProxyRequestContext*>(data);
      preq->startProcessing();
    } break;

    case ProxyMessage::Type::OLD_CONFIG: {
      auto oldConfig = reinterpret_cast<old_config_req_t<RouterInfo>*>(data);
      delete oldConfig;
    } break;

    case ProxyMessage::Type::SHUTDOWN:
      /*
       * No-op. We just wanted to wake this event base up so that
       * it can exit event loop and check router->shutdown
       */
      break;
  }
}

template <class RouterInfo>
void Proxy<RouterInfo>::routeHandlesProcessRequest(
    const McStatsRequest& req,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, McStatsRequest>> ctx) {
  ctx->sendReply(stats_reply(this, req.key().fullKey()));
}

template <class RouterInfo>
void Proxy<RouterInfo>::routeHandlesProcessRequest(
    const McVersionRequest&,
    std::unique_ptr<ProxyRequestContextTyped<RouterInfo, McVersionRequest>>
        ctx) {
  McVersionReply reply(mc_res_ok);
  reply.value() =
      folly::IOBuf(folly::IOBuf::COPY_BUFFER, MCROUTER_PACKAGE_STRING);
  ctx->sendReply(std::move(reply));
}

template <class RouterInfo>
void Proxy<RouterInfo>::pump() {
  auto numPriorities = static_cast<int>(ProxyRequestPriority::kNumPriorities);
  for (int i = 0; i < numPriorities; ++i) {
    auto& queue = waitingRequests_[i];
    while (numRequestsProcessing_ <
               router().opts().proxy_max_inflight_requests &&
           !queue.empty()) {
      --numRequestsWaiting_;
      auto w = queue.popFront();
      stats().decrement(proxy_reqs_waiting_stat);

      w->process(this);
    }
  }
}

template <class RouterInfo>
void proxy_config_swap(
    Proxy<RouterInfo>* proxy,
    std::shared_ptr<ProxyConfig<RouterInfo>> config) {
  auto oldConfig = proxy->swapConfig(std::move(config));
  proxy->stats().setValue(config_last_success_stat, time(nullptr));

  if (oldConfig) {
    auto configReq = new old_config_req_t<RouterInfo>(std::move(oldConfig));
    proxy->sendMessage(ProxyMessage::Type::OLD_CONFIG, configReq);
  }
}

template <class RouterInfo>
template <class Request>
typename std::enable_if<TNotRateLimited<Request>::value, bool>::type
Proxy<RouterInfo>::rateLimited(ProxyRequestPriority priority, const Request&)
    const {
  return false;
}

template <class RouterInfo>
template <class Request>
typename std::enable_if<!TNotRateLimited<Request>::value, bool>::type
Proxy<RouterInfo>::rateLimited(ProxyRequestPriority priority, const Request&)
    const {
  if (!getRouterOptions().proxy_max_inflight_requests) {
    return false;
  }

  if (waitingRequests_[static_cast<int>(priority)].empty() &&
      numRequestsProcessing_ < getRouterOptions().proxy_max_inflight_requests) {
    return false;
  }

  return true;
}

} // mcrouter
} // memcache
} // facebook
