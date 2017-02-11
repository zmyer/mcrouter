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

#include <memory>

#include <folly/fibers/FiberManager.h>

#include "mcrouter/McrouterFiberContext.h"
#include "mcrouter/McrouterLogFailure.h"

namespace facebook {
namespace memcache {
namespace mcrouter {

template <class RouterInfo, class ShadowPolicy>
template <class Request>
void ShadowRoute<RouterInfo, ShadowPolicy>::dispatchShadowRequest(
    std::shared_ptr<typename RouterInfo::RouteHandleIf> shadow,
    std::shared_ptr<Request> adjustedReq) const {
  folly::fibers::addTask(
      [ shadow = std::move(shadow), adjustedReq = std::move(adjustedReq) ]() {
        // we don't want to spool shadow requests
        fiber_local<RouterInfo>::clearAsynclogName();
        fiber_local<RouterInfo>::addRequestClass(RequestClass::kShadow);
        shadow->route(*adjustedReq);
      });
}

template <class RouterInfo>
std::shared_ptr<typename RouterInfo::RouteHandleIf> makeShadowRouteDefault(
    std::shared_ptr<typename RouterInfo::RouteHandleIf> normalRoute,
    ShadowData<RouterInfo> shadowData,
    DefaultShadowPolicy shadowPolicy) {
  return makeRouteHandleWithInfo<RouterInfo, ShadowRoute, DefaultShadowPolicy>(
      std::move(normalRoute), std::move(shadowData), std::move(shadowPolicy));
}

template <class RouterInfo>
std::vector<std::shared_ptr<typename RouterInfo::RouteHandleIf>>
makeShadowRoutes(
    RouteHandleFactory<typename RouterInfo::RouteHandleIf>& factory,
    const folly::dynamic& json,
    std::vector<std::shared_ptr<typename RouterInfo::RouteHandleIf>> children,
    ProxyBase& proxy,
    ExtraRouteHandleProviderIf<RouterInfo>& extraProvider) {
  folly::StringPiece shadowPolicy = "default";
  if (auto jshadow_policy = json.get_ptr("shadow_policy")) {
    checkLogic(
        jshadow_policy->isString(),
        "ShadowRoute: shadow_policy is not a string");
    shadowPolicy = jshadow_policy->stringPiece();
  }

  auto jshadows = json.get_ptr("shadows");
  checkLogic(jshadows, "ShadowRoute: route doesn't contain shadows field");

  if (!jshadows->isArray()) {
    MC_LOG_FAILURE(
        proxy.router().opts(),
        failure::Category::kInvalidConfig,
        "ShadowRoute: shadows specified in route is not an array");
    return children;
  }

  ShadowData<RouterInfo> data;
  for (auto& shadow : *jshadows) {
    if (!shadow.isObject()) {
      MC_LOG_FAILURE(
          proxy.router().opts(),
          failure::Category::kInvalidConfig,
          "ShadowRoute: shadow is not an object");
      continue;
    }
    auto jtarget = shadow.get_ptr("target");
    if (!jtarget) {
      MC_LOG_FAILURE(
          proxy.router().opts(),
          failure::Category::kInvalidConfig,
          "ShadowRoute shadows: no target for shadow");
      continue;
    }
    try {
      auto s = ShadowSettings::create(shadow, proxy.router());
      if (s) {
        data.emplace_back(factory.create(*jtarget), std::move(s));
      }
    } catch (const std::exception& e) {
      MC_LOG_FAILURE(
          proxy.router().opts(),
          failure::Category::kInvalidConfig,
          "Can not create shadow for ShadowRoute: {}",
          e.what());
    }
  }
  for (size_t i = 0; i < children.size(); ++i) {
    ShadowData<RouterInfo> childrenShadows;
    for (const auto& shadowData : data) {
      if (shadowData.second->startIndex() <= i &&
          i < shadowData.second->endIndex()) {
        childrenShadows.push_back(shadowData);
      }
    }
    if (!childrenShadows.empty()) {
      childrenShadows.shrink_to_fit();
      children[i] = extraProvider.makeShadow(
          proxy,
          std::move(children[i]),
          std::move(childrenShadows),
          shadowPolicy);
    }
  }
  return children;
}

template <class RouterInfo>
std::vector<std::shared_ptr<typename RouterInfo::RouteHandleIf>>
makeShadowRoutes(
    RouteHandleFactory<typename RouterInfo::RouteHandleIf>& factory,
    const folly::dynamic& json,
    ProxyBase& proxy,
    ExtraRouteHandleProviderIf<RouterInfo>& extraProvider) {
  checkLogic(json.isObject(), "ShadowRoute should be an object");
  const auto jchildren = json.get_ptr("children");
  checkLogic(jchildren, "ShadowRoute: children not found");
  auto children = factory.createList(*jchildren);
  if (json.count("shadows")) {
    children = makeShadowRoutes(
        factory, json, std::move(children), proxy, extraProvider);
  }
  return children;
}

} // mcrouter
} // memcache
} // facebook
