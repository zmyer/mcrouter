/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */

/*
 *  THIS FILE IS AUTOGENERATED. DO NOT MODIFY IT; ALL CHANGES WILL BE LOST IN
 *  VAIN.
 *
 *  @generated
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
#include <mcrouter/lib/carbon/TypeList.h>
#include <mcrouter/lib/carbon/Variant.h>

#include "mcrouter/lib/carbon/test/a/gen/AMessages.h"
#include "mcrouter/lib/network/gen/CommonMessages.h"

#include "mcrouter/lib/carbon/test/TestUserType.h"

namespace carbon {
namespace test {

class TestReply;

class TestRequest : public carbon::RequestCommon {
 public:
  using reply_type = TestReply;
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = true;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 69;
  static constexpr const char* name = "test";

  TestRequest() = default;
  TestRequest(const TestRequest&) = default;
  TestRequest& operator=(const TestRequest&) = default;
  TestRequest(TestRequest&&) = default;
  TestRequest& operator=(TestRequest&&) = default;
  explicit TestRequest(folly::StringPiece sp) : key_(sp) {}
  explicit TestRequest(folly::IOBuf&& carbonKey) : key_(std::move(carbonKey)) {}

  SimpleStruct& asBase() {
    return _carbon_simplestruct_;
  }
  const SimpleStruct& asBase() const {
    return _carbon_simplestruct_;
  }
  BaseStruct& asBaseStruct() {
    return asBase().asBaseStruct();
  }
  const BaseStruct& asBaseStruct() const {
    return asBase().asBaseStruct();
  }
  int32_t int32Member() const {
    return _carbon_simplestruct_.int32Member();
  }
  int32_t& int32Member() {
    return _carbon_simplestruct_.int32Member();
  }
  const std::string& stringMember() const {
    return _carbon_simplestruct_.stringMember();
  }
  std::string& stringMember() {
    return _carbon_simplestruct_.stringMember();
  }
  const test2::util::SimpleEnum& enumMember() const {
    return _carbon_simplestruct_.enumMember();
  }
  test2::util::SimpleEnum& enumMember() {
    return _carbon_simplestruct_.enumMember();
  }
  const std::vector<test2::util::SimpleStruct>& vectorMember() const {
    return _carbon_simplestruct_.vectorMember();
  }
  std::vector<test2::util::SimpleStruct>& vectorMember() {
    return _carbon_simplestruct_.vectorMember();
  }
  int64_t baseInt64Member() const {
    return _carbon_simplestruct_.baseInt64Member();
  }
  int64_t& baseInt64Member() {
    return _carbon_simplestruct_.baseInt64Member();
  }
  const carbon::Keys<folly::IOBuf>& key() const {
    return key_;
  }
  carbon::Keys<folly::IOBuf>& key() {
    return key_;
  }
  const test2::util::SimpleEnum& testEnum() const {
    return testEnum_;
  }
  test2::util::SimpleEnum& testEnum() {
    return testEnum_;
  }
  bool testBool() const {
    return testBool_;
  }
  bool& testBool() {
    return testBool_;
  }
  char testChar() const {
    return testChar_;
  }
  char& testChar() {
    return testChar_;
  }
  int8_t testInt8() const {
    return testInt8_;
  }
  int8_t& testInt8() {
    return testInt8_;
  }
  int16_t testInt16() const {
    return testInt16_;
  }
  int16_t& testInt16() {
    return testInt16_;
  }
  int32_t testInt32() const {
    return testInt32_;
  }
  int32_t& testInt32() {
    return testInt32_;
  }
  int64_t testInt64() const {
    return testInt64_;
  }
  int64_t& testInt64() {
    return testInt64_;
  }
  uint8_t testUInt8() const {
    return testUInt8_;
  }
  uint8_t& testUInt8() {
    return testUInt8_;
  }
  uint16_t testUInt16() const {
    return testUInt16_;
  }
  uint16_t& testUInt16() {
    return testUInt16_;
  }
  uint32_t testUInt32() const {
    return testUInt32_;
  }
  uint32_t& testUInt32() {
    return testUInt32_;
  }
  uint64_t testUInt64() const {
    return testUInt64_;
  }
  uint64_t& testUInt64() {
    return testUInt64_;
  }
  float testFloat() const {
    return testFloat_;
  }
  float& testFloat() {
    return testFloat_;
  }
  double testDouble() const {
    return testDouble_;
  }
  double& testDouble() {
    return testDouble_;
  }
  const std::string& testShortString() const {
    return testShortString_;
  }
  std::string& testShortString() {
    return testShortString_;
  }
  const std::string& testLongString() const {
    return testLongString_;
  }
  std::string& testLongString() {
    return testLongString_;
  }
  const folly::IOBuf& testIobuf() const {
    return testIobuf_;
  }
  folly::IOBuf& testIobuf() {
    return testIobuf_;
  }
  const SimpleStruct& testStruct() const {
    return testStruct_;
  }
  SimpleStruct& testStruct() {
    return testStruct_;
  }
  const std::vector<std::string>& testList() const {
    return testList_;
  }
  std::vector<std::string>& testList() {
    return testList_;
  }
  const folly::Optional<std::string>& testOptionalString() const {
    return testOptionalString_;
  }
  folly::Optional<std::string>& testOptionalString() {
    return testOptionalString_;
  }
  const folly::Optional<folly::IOBuf>& testOptionalIobuf() const {
    return testOptionalIobuf_;
  }
  folly::Optional<folly::IOBuf>& testOptionalIobuf() {
    return testOptionalIobuf_;
  }
  const std::vector<test2::util::SimpleEnum>& testEnumVec() const {
    return testEnumVec_;
  }
  std::vector<test2::util::SimpleEnum>& testEnumVec() {
    return testEnumVec_;
  }
  const test2::util::SimpleUnion& testUnion() const {
    return testUnion_;
  }
  test2::util::SimpleUnion& testUnion() {
    return testUnion_;
  }
  const std::vector<std::vector<uint64_t>>& testNestedVec() const {
    return testNestedVec_;
  }
  std::vector<std::vector<uint64_t>>& testNestedVec() {
    return testNestedVec_;
  }
  const std::unordered_map<std::string, std::string>& testUMap() const {
    return testUMap_;
  }
  std::unordered_map<std::string, std::string>& testUMap() {
    return testUMap_;
  }
  const std::map<double, double>& testMap() const {
    return testMap_;
  }
  std::map<double, double>& testMap() {
    return testMap_;
  }
  const std::map<std::string, std::vector<uint16_t>>& testComplexMap() const {
    return testComplexMap_;
  }
  std::map<std::string, std::vector<uint16_t>>& testComplexMap() {
    return testComplexMap_;
  }
  const std::unordered_set<std::string>& testUSet() const {
    return testUSet_;
  }
  std::unordered_set<std::string>& testUSet() {
    return testUSet_;
  }
  const std::set<uint64_t>& testSet() const {
    return testSet_;
  }
  std::set<uint64_t>& testSet() {
    return testSet_;
  }
  const folly::Optional<bool>& testOptionalBool() const {
    return testOptionalBool_;
  }
  folly::Optional<bool>& testOptionalBool() {
    return testOptionalBool_;
  }
  const std::vector<folly::Optional<std::string>>& testOptionalVec() const {
    return testOptionalVec_;
  }
  std::vector<folly::Optional<std::string>>& testOptionalVec() {
    return testOptionalVec_;
  }
  const std::vector<folly::IOBuf>& testIOBufList() const {
    return testIOBufList_;
  }
  std::vector<folly::IOBuf>& testIOBufList() {
    return testIOBufList_;
  }
  const UserType& testType() const {
    return testType_;
  }
  UserType& testType() {
    return testType_;
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
  SimpleStruct _carbon_simplestruct_;
  carbon::Keys<folly::IOBuf> key_;
  test2::util::SimpleEnum testEnum_{test2::util::SimpleEnum::Twenty};
  int64_t testInt64_{0};
  uint64_t testUInt64_{0};
  double testDouble_{0.0};
  std::string testShortString_;
  std::string testLongString_;
  folly::IOBuf testIobuf_;
  SimpleStruct testStruct_;
  std::vector<std::string> testList_;
  folly::Optional<std::string> testOptionalString_;
  folly::Optional<folly::IOBuf> testOptionalIobuf_;
  std::vector<test2::util::SimpleEnum> testEnumVec_;
  test2::util::SimpleUnion testUnion_;
  std::vector<std::vector<uint64_t>> testNestedVec_;
  std::unordered_map<std::string, std::string> testUMap_;
  std::map<double, double> testMap_;
  std::map<std::string, std::vector<uint16_t>> testComplexMap_;
  std::unordered_set<std::string> testUSet_;
  std::set<uint64_t> testSet_;
  folly::Optional<bool> testOptionalBool_;
  std::vector<folly::Optional<std::string>> testOptionalVec_;
  std::vector<folly::IOBuf> testIOBufList_;
  UserType testType_;
  int32_t testInt32_{0};
  uint32_t testUInt32_{0};
  float testFloat_{0.0};
  int16_t testInt16_{0};
  uint16_t testUInt16_{0};
  bool testBool_{false};
  char testChar_{'\0'};
  int8_t testInt8_{0};
  uint8_t testUInt8_{0};
};

class TestReply : public carbon::ReplyCommon {
 public:
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = false;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 70;

  TestReply() = default;
  TestReply(const TestReply&) = default;
  TestReply& operator=(const TestReply&) = default;
  TestReply(TestReply&&) = default;
  TestReply& operator=(TestReply&&) = default;
  explicit TestReply(carbon::Result carbonResult) : result_(carbonResult) {}

  carbon::Result result() const {
    return result_;
  }
  carbon::Result& result() {
    return result_;
  }
  int32_t valInt32() const {
    return valInt32_;
  }
  int32_t& valInt32() {
    return valInt32_;
  }
  int64_t valInt64() const {
    return valInt64_;
  }
  int64_t& valInt64() {
    return valInt64_;
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
  int64_t valInt64_{0};
  int32_t valInt32_{0};
  carbon::Result result_{mc_res_unknown};
};

class TestReplyStringKey;

class TestRequestStringKey : public carbon::RequestCommon {
 public:
  using reply_type = TestReplyStringKey;
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = true;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 71;
  static constexpr const char* name = "testStringKey";

  TestRequestStringKey() = default;
  TestRequestStringKey(const TestRequestStringKey&) = default;
  TestRequestStringKey& operator=(const TestRequestStringKey&) = default;
  TestRequestStringKey(TestRequestStringKey&&) = default;
  TestRequestStringKey& operator=(TestRequestStringKey&&) = default;
  explicit TestRequestStringKey(folly::StringPiece sp) : key_(sp) {}
  explicit TestRequestStringKey(std::string&& carbonKey)
      : key_(std::move(carbonKey)) {}
  explicit TestRequestStringKey(const char* key) : key_(key) {}

  const carbon::Keys<std::string>& key() const {
    return key_;
  }
  carbon::Keys<std::string>& key() {
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
  carbon::Keys<std::string> key_;
};

class TestReplyStringKey : public carbon::ReplyCommon {
 public:
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = false;
  static constexpr bool hasValue = false;
  static constexpr size_t typeId = 72;

  TestReplyStringKey() = default;
  TestReplyStringKey(const TestReplyStringKey&) = default;
  TestReplyStringKey& operator=(const TestReplyStringKey&) = default;
  TestReplyStringKey(TestReplyStringKey&&) = default;
  TestReplyStringKey& operator=(TestReplyStringKey&&) = default;
  explicit TestReplyStringKey(carbon::Result carbonResult)
      : result_(carbonResult) {}

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

class TestOptionalBool {
 public:
  static constexpr bool hasExptime = false;
  static constexpr bool hasFlags = false;
  static constexpr bool hasKey = false;
  static constexpr bool hasValue = false;

  TestOptionalBool() = default;
  TestOptionalBool(const TestOptionalBool&) = default;
  TestOptionalBool& operator=(const TestOptionalBool&) = default;
  TestOptionalBool(TestOptionalBool&&) = default;
  TestOptionalBool& operator=(TestOptionalBool&&) = default;

  const folly::Optional<bool>& optionalBool() const {
    return optionalBool_;
  }
  folly::Optional<bool>& optionalBool() {
    return optionalBool_;
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
  folly::Optional<bool> optionalBool_;
};

class TestOptionalUnion {
 private:
  using _IdTypeMap = carbon::List<
      facebook::memcache::KV<1, folly::Optional<int64_t>>,
      facebook::memcache::KV<2, folly::Optional<bool>>,
      facebook::memcache::KV<3, folly::Optional<std::string>>>;

 public:
  TestOptionalUnion() = default;
  TestOptionalUnion(const TestOptionalUnion&) = default;
  TestOptionalUnion& operator=(const TestOptionalUnion&) = default;
  TestOptionalUnion(TestOptionalUnion&&) = default;
  TestOptionalUnion& operator=(TestOptionalUnion&&) = default;

  uint32_t which() const {
    return _which_;
  }

  folly::Optional<int64_t>& umember1() {
    if (_which_ == 0) {
      return emplace<1>();
    }
    if (_which_ != 1) {
      throw std::runtime_error(
          "umember1 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<int64_t>>();
  }
  const folly::Optional<int64_t>& umember1() const {
    if (_which_ != 1) {
      throw std::runtime_error(
          "umember1 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<int64_t>>();
  }

  folly::Optional<bool>& umember2() {
    if (_which_ == 0) {
      return emplace<2>();
    }
    if (_which_ != 2) {
      throw std::runtime_error(
          "umember2 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<bool>>();
  }
  const folly::Optional<bool>& umember2() const {
    if (_which_ != 2) {
      throw std::runtime_error(
          "umember2 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<bool>>();
  }

  folly::Optional<std::string>& umember3() {
    if (_which_ == 0) {
      return emplace<3>();
    }
    if (_which_ != 3) {
      throw std::runtime_error(
          "umember3 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<std::string>>();
  }
  const folly::Optional<std::string>& umember3() const {
    if (_which_ != 3) {
      throw std::runtime_error(
          "umember3 is not set in union TestOptionalUnion.");
    }
    return _carbon_variant.get<folly::Optional<std::string>>();
  }

  template <
      uint32_t id,
      class C = typename carbon::FindByKey<id, _IdTypeMap>::type>
  C& get() {
    if (id != _which_) {
      throw std::runtime_error("Type id is not set in union SimpleUnion.");
    }
    return _carbon_variant.get<C>();
  }

  template <
      uint32_t id,
      class C = typename carbon::FindByKey<id, _IdTypeMap>::type>
  const C& get() const {
    if (id != _which_) {
      throw std::runtime_error("Type id is not set in union SimpleUnion.");
    }
    return _carbon_variant.get<C>();
  }

  /* Note: Emplace invalidates all previous accessor references.
   * Please exercise caution.
   */
  template <
      uint32_t id,
      class... Args,
      class C = typename carbon::FindByKey<id, _IdTypeMap>::type>
  C& emplace(Args&&... args) {
    _which_ = id;
    return _carbon_variant.emplace<C>(std::forward<Args>(args)...);
  }

  void serialize(carbon::CarbonProtocolWriter& writer) const;

  void deserialize(carbon::CarbonProtocolReader& reader);

  template <class V>
  void visitFields(V&& v);
  template <class V>
  void visitFields(V&& v) const;
  template <class V>
  void foreachMember(V&& v);
  template <class V>
  void foreachMember(V&& v) const;

 private:
  carbon::Variant<
      folly::Optional<int64_t>,
      folly::Optional<bool>,
      folly::Optional<std::string>>
      _carbon_variant;

  uint32_t _which_{0};
};
} // namespace test
} // namespace carbon

#include "CarbonTestMessages-inl.h"
