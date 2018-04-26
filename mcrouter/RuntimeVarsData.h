/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#pragma once

#include <string>
#include <unordered_map>

#include <folly/Range.h>
#include <folly/dynamic.h>

namespace facebook {
namespace memcache {
namespace mcrouter {

class RuntimeVarsData {
 public:
  RuntimeVarsData() = default;
  explicit RuntimeVarsData(folly::StringPiece json);

  /**
   * Returns the value of the variable with key = name.
   *
   * @param name key of the data to be retrieved
   * @return Variable value, or null if key not found
   */
  folly::dynamic getVariableByName(const std::string& name) const;

 private:
  std::unordered_map<std::string, folly::dynamic> configData_;
};
}
}
} // facebook::memcache::mcrouter
