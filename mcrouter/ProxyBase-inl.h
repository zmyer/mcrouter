/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <folly/Memory.h>
#include <folly/Random.h>
#include <folly/fibers/EventBaseLoopController.h>

#include "mcrouter/CarbonRouterInstanceBase.h"
#include "mcrouter/McrouterFiberContext.h"
#include "mcrouter/ProxyDestinationMap.h"

namespace facebook {
namespace memcache {
namespace mcrouter {

template <class RouterInfo>
ProxyBase::ProxyBase(
    CarbonRouterInstanceBase& rtr,
    size_t id,
    folly::EventBase& evb,
    RouterInfo /* tag */)
    : router_(rtr),
      id_(id),
      eventBase_(evb),
      fiberManager_(
          typename fiber_local<RouterInfo>::ContextTypeTag(),
          folly::make_unique<folly::fibers::EventBaseLoopController>(),
          getFiberManagerOptions(router_.opts())),
      asyncLog_(router_.opts()),
      destinationMap_(folly::make_unique<ProxyDestinationMap>(this)) {
  // Setup a full random seed sequence
  folly::Random::seed(randomGenerator_);

  statsContainer_ = folly::make_unique<ProxyStatsContainer>(*this);
}

} // mcrouter
} // memcache
} // facebook
