// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: slankdev.proto

#include "slankdev.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace slankdev {
class HelloRequestDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<HelloRequest>
      _instance;
} _HelloRequest_default_instance_;
class HelloReplyDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<HelloReply>
      _instance;
} _HelloReply_default_instance_;
}  // namespace slankdev
namespace protobuf_slankdev_2eproto {
void InitDefaultsHelloRequestImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::slankdev::_HelloRequest_default_instance_;
    new (ptr) ::slankdev::HelloRequest();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::slankdev::HelloRequest::InitAsDefaultInstance();
}

void InitDefaultsHelloRequest() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsHelloRequestImpl);
}

void InitDefaultsHelloReplyImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::slankdev::_HelloReply_default_instance_;
    new (ptr) ::slankdev::HelloReply();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::slankdev::HelloReply::InitAsDefaultInstance();
}

void InitDefaultsHelloReply() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsHelloReplyImpl);
}

::google::protobuf::Metadata file_level_metadata[2];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::slankdev::HelloRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::slankdev::HelloRequest, name_),
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::slankdev::HelloReply, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::slankdev::HelloReply, message_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::slankdev::HelloRequest)},
  { 6, -1, sizeof(::slankdev::HelloReply)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::slankdev::_HelloRequest_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::slankdev::_HelloReply_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "slankdev.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\016slankdev.proto\022\010slankdev\"\034\n\014HelloReque"
      "st\022\014\n\004name\030\001 \001(\t\"\035\n\nHelloReply\022\017\n\007messag"
      "e\030\001 \001(\t2E\n\007Greeter\022:\n\010SayHello\022\026.slankde"
      "v.HelloRequest\032\024.slankdev.HelloReply\"\000b\006"
      "proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 166);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "slankdev.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_slankdev_2eproto
namespace slankdev {

// ===================================================================

void HelloRequest::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int HelloRequest::kNameFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

HelloRequest::HelloRequest()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_slankdev_2eproto::InitDefaultsHelloRequest();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:slankdev.HelloRequest)
}
HelloRequest::HelloRequest(const HelloRequest& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.name().size() > 0) {
    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
  // @@protoc_insertion_point(copy_constructor:slankdev.HelloRequest)
}

void HelloRequest::SharedCtor() {
  name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _cached_size_ = 0;
}

HelloRequest::~HelloRequest() {
  // @@protoc_insertion_point(destructor:slankdev.HelloRequest)
  SharedDtor();
}

void HelloRequest::SharedDtor() {
  name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void HelloRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* HelloRequest::descriptor() {
  ::protobuf_slankdev_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_slankdev_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const HelloRequest& HelloRequest::default_instance() {
  ::protobuf_slankdev_2eproto::InitDefaultsHelloRequest();
  return *internal_default_instance();
}

HelloRequest* HelloRequest::New(::google::protobuf::Arena* arena) const {
  HelloRequest* n = new HelloRequest;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void HelloRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:slankdev.HelloRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

bool HelloRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:slankdev.HelloRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string name = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->name().data(), static_cast<int>(this->name().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "slankdev.HelloRequest.name"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:slankdev.HelloRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:slankdev.HelloRequest)
  return false;
#undef DO_
}

void HelloRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:slankdev.HelloRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string name = 1;
  if (this->name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "slankdev.HelloRequest.name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:slankdev.HelloRequest)
}

::google::protobuf::uint8* HelloRequest::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:slankdev.HelloRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string name = 1;
  if (this->name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->name().data(), static_cast<int>(this->name().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "slankdev.HelloRequest.name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:slankdev.HelloRequest)
  return target;
}

size_t HelloRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:slankdev.HelloRequest)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string name = 1;
  if (this->name().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->name());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HelloRequest::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:slankdev.HelloRequest)
  GOOGLE_DCHECK_NE(&from, this);
  const HelloRequest* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const HelloRequest>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:slankdev.HelloRequest)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:slankdev.HelloRequest)
    MergeFrom(*source);
  }
}

void HelloRequest::MergeFrom(const HelloRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:slankdev.HelloRequest)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.name().size() > 0) {

    name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.name_);
  }
}

void HelloRequest::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:slankdev.HelloRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HelloRequest::CopyFrom(const HelloRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:slankdev.HelloRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HelloRequest::IsInitialized() const {
  return true;
}

void HelloRequest::Swap(HelloRequest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void HelloRequest::InternalSwap(HelloRequest* other) {
  using std::swap;
  name_.Swap(&other->name_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata HelloRequest::GetMetadata() const {
  protobuf_slankdev_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_slankdev_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void HelloReply::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int HelloReply::kMessageFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

HelloReply::HelloReply()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_slankdev_2eproto::InitDefaultsHelloReply();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:slankdev.HelloReply)
}
HelloReply::HelloReply(const HelloReply& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.message().size() > 0) {
    message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
  }
  // @@protoc_insertion_point(copy_constructor:slankdev.HelloReply)
}

void HelloReply::SharedCtor() {
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _cached_size_ = 0;
}

HelloReply::~HelloReply() {
  // @@protoc_insertion_point(destructor:slankdev.HelloReply)
  SharedDtor();
}

void HelloReply::SharedDtor() {
  message_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void HelloReply::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* HelloReply::descriptor() {
  ::protobuf_slankdev_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_slankdev_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const HelloReply& HelloReply::default_instance() {
  ::protobuf_slankdev_2eproto::InitDefaultsHelloReply();
  return *internal_default_instance();
}

HelloReply* HelloReply::New(::google::protobuf::Arena* arena) const {
  HelloReply* n = new HelloReply;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void HelloReply::Clear() {
// @@protoc_insertion_point(message_clear_start:slankdev.HelloReply)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

bool HelloReply::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:slankdev.HelloReply)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string message = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->message().data(), static_cast<int>(this->message().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "slankdev.HelloReply.message"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:slankdev.HelloReply)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:slankdev.HelloReply)
  return false;
#undef DO_
}

void HelloReply::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:slankdev.HelloReply)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), static_cast<int>(this->message().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "slankdev.HelloReply.message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->message(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:slankdev.HelloReply)
}

::google::protobuf::uint8* HelloReply::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:slankdev.HelloReply)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), static_cast<int>(this->message().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "slankdev.HelloReply.message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->message(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:slankdev.HelloReply)
  return target;
}

size_t HelloReply::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:slankdev.HelloReply)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string message = 1;
  if (this->message().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HelloReply::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:slankdev.HelloReply)
  GOOGLE_DCHECK_NE(&from, this);
  const HelloReply* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const HelloReply>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:slankdev.HelloReply)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:slankdev.HelloReply)
    MergeFrom(*source);
  }
}

void HelloReply::MergeFrom(const HelloReply& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:slankdev.HelloReply)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.message().size() > 0) {

    message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
  }
}

void HelloReply::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:slankdev.HelloReply)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HelloReply::CopyFrom(const HelloReply& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:slankdev.HelloReply)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HelloReply::IsInitialized() const {
  return true;
}

void HelloReply::Swap(HelloReply* other) {
  if (other == this) return;
  InternalSwap(other);
}
void HelloReply::InternalSwap(HelloReply* other) {
  using std::swap;
  message_.Swap(&other->message_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata HelloReply::GetMetadata() const {
  protobuf_slankdev_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_slankdev_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace slankdev

// @@protoc_insertion_point(global_scope)
