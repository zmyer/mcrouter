/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

/*
 *  THIS FILE IS AUTOGENERATED. DO NOT MODIFY IT; ALL CHANGES WILL BE LOST IN
 *  VAIN.
 *
 */
#pragma once

#include <memory>
#include <string>
#include <utility>

#include <folly/Optional.h>
#include <folly/io/IOBuf.h>
#include <mcrouter/lib/carbon/CarbonProtocolReader.h>
#include <mcrouter/lib/carbon/CarbonProtocolWriter.h>
#include <mcrouter/lib/carbon/CommonSerializationTraits.h>
#include <mcrouter/lib/carbon/Keys.h>
#include <mcrouter/lib/carbon/ReplyCommon.h>
#include <mcrouter/lib/carbon/RequestCommon.h>
#include <mcrouter/lib/carbon/RequestReplyUtil.h>
#include <mcrouter/lib/carbon/Result.h>

#include "mcrouter/lib/network/gen/CommonMessages.h"

namespace hellogoodbye {

class HelloReply;

class HelloRequest : public carbon::RequestCommon {
 public:
  using reply_type = HelloReply;
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = true;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 65;
  static constexpr const char* name = "hello";

  HelloRequest() = default;
  HelloRequest(const HelloRequest&) = default;
  HelloRequest& operator=(const HelloRequest&) = default;
  HelloRequest(HelloRequest&&) = default;
  HelloRequest& operator=(HelloRequest&&) = default;
  explicit HelloRequest(folly::StringPiece sp) : key_(sp) {}
  explicit HelloRequest(folly::IOBuf&& carbonKey)
      : key_(std::move(carbonKey)) {}

  const carbon::Keys<folly::IOBuf>& key() const {
    return key_;
  }
  carbon::Keys<folly::IOBuf>& key() {
    return key_;
  }
  uint64_t flags() const {
    return 0;
  }
  int32_t exptime() const {
    return 0;
  }

  void serialize(carbon::CarbonProtocolWriter& writer) const;

  void deserialize(carbon::CarbonProtocolReader& reader);

  template <class V>
  void visitFields(V&& v);
  template <class V>
  void visitFields(V&& v) const;

 private:
  carbon::Keys<folly::IOBuf> key_;
};

class HelloReply : public carbon::ReplyCommon {
 public:
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = false;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 66;

  HelloReply() = default;
  HelloReply(const HelloReply&) = default;
  HelloReply& operator=(const HelloReply&) = default;
  HelloReply(HelloReply&&) = default;
  HelloReply& operator=(HelloReply&&) = default;
  explicit HelloReply(carbon::Result carbonResult) : result_(carbonResult) {}

  carbon::Result result() const {
    return result_;
  }
  carbon::Result& result() {
    return result_;
  }
  uint64_t flags() const {
    return 0;
  }
  int32_t exptime() const {
    return 0;
  }

  void serialize(carbon::CarbonProtocolWriter& writer) const;

  void deserialize(carbon::CarbonProtocolReader& reader);

  template <class V>
  void visitFields(V&& v);
  template <class V>
  void visitFields(V&& v) const;

 private:
  carbon::Result result_{mc_res_unknown};
};

class GoodbyeReply;

class GoodbyeRequest : public carbon::RequestCommon {
 public:
  using reply_type = GoodbyeReply;
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = true;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 67;
  static constexpr const char* name = "goodbye";

  GoodbyeRequest() = default;
  GoodbyeRequest(const GoodbyeRequest&) = default;
  GoodbyeRequest& operator=(const GoodbyeRequest&) = default;
  GoodbyeRequest(GoodbyeRequest&&) = default;
  GoodbyeRequest& operator=(GoodbyeRequest&&) = default;
  explicit GoodbyeRequest(folly::StringPiece sp) : key_(sp) {}
  explicit GoodbyeRequest(folly::IOBuf&& carbonKey)
      : key_(std::move(carbonKey)) {}

  const carbon::Keys<folly::IOBuf>& key() const {
    return key_;
  }
  carbon::Keys<folly::IOBuf>& key() {
    return key_;
  }
  uint64_t flags() const {
    return 0;
  }
  int32_t exptime() const {
    return 0;
  }

  void serialize(carbon::CarbonProtocolWriter& writer) const;

  void deserialize(carbon::CarbonProtocolReader& reader);

  template <class V>
  void visitFields(V&& v);
  template <class V>
  void visitFields(V&& v) const;

 private:
  carbon::Keys<folly::IOBuf> key_;
};

class GoodbyeReply : public carbon::ReplyCommon {
 public:
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = false;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 68;

  GoodbyeReply() = default;
  GoodbyeReply(const GoodbyeReply&) = default;
  GoodbyeReply& operator=(const GoodbyeReply&) = default;
  GoodbyeReply(GoodbyeReply&&) = default;
  GoodbyeReply& operator=(GoodbyeReply&&) = default;
  explicit GoodbyeReply(carbon::Result carbonResult) : result_(carbonResult) {}

  carbon::Result result() const {
    return result_;
  }
  carbon::Result& result() {
    return result_;
  }
  const std::string& message() const {
    return message_;
  }
  std::string& message() {
    return message_;
  }
  uint64_t flags() const {
    return 0;
  }
  int32_t exptime() const {
    return 0;
  }

  void serialize(carbon::CarbonProtocolWriter& writer) const;

  void deserialize(carbon::CarbonProtocolReader& reader);

  template <class V>
  void visitFields(V&& v);
  template <class V>
  void visitFields(V&& v) const;

 private:
  carbon::Result result_{mc_res_unknown};
  std::string message_;
};

} // hellogoodbye

#include "HelloGoodbyeMessages-inl.h"
