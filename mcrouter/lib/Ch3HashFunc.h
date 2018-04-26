/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#pragma once

#include <folly/Range.h>

#include "mcrouter/lib/fbi/hash.h"

namespace folly {
struct dynamic;
} // folly

namespace facebook {
namespace memcache {

/* CH3 consistent hashing function object */
class Ch3HashFunc {
 public:
  explicit Ch3HashFunc(size_t n) : n_(n) {
    if (!n_ || n_ > furc_maximum_pool_size()) {
      throw std::logic_error("Pool size out of range for Ch3");
    }
  }

  size_t operator()(folly::StringPiece hashable) const {
    return furc_hash(hashable.data(), hashable.size(), n_);
  }

  static const char* type() {
    return "Ch3";
  }

 private:
  size_t n_;
};
}
} // facebook::memcache
