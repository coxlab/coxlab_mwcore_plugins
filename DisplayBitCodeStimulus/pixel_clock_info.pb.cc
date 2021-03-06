// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pixel_clock_info.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "pixel_clock_info.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* PixelClockInfoBuffer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PixelClockInfoBuffer_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_pixel_5fclock_5finfo_2eproto() {
  protobuf_AddDesc_pixel_5fclock_5finfo_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "pixel_clock_info.proto");
  GOOGLE_CHECK(file != NULL);
  PixelClockInfoBuffer_descriptor_ = file->message_type(0);
  static const int PixelClockInfoBuffer_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PixelClockInfoBuffer, channel_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PixelClockInfoBuffer, time_stamp_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PixelClockInfoBuffer, direction_),
  };
  PixelClockInfoBuffer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PixelClockInfoBuffer_descriptor_,
      PixelClockInfoBuffer::default_instance_,
      PixelClockInfoBuffer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PixelClockInfoBuffer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PixelClockInfoBuffer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PixelClockInfoBuffer));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_pixel_5fclock_5finfo_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PixelClockInfoBuffer_descriptor_, &PixelClockInfoBuffer::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_pixel_5fclock_5finfo_2eproto() {
  delete PixelClockInfoBuffer::default_instance_;
  delete PixelClockInfoBuffer_reflection_;
}

void protobuf_AddDesc_pixel_5fclock_5finfo_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026pixel_clock_info.proto\"Q\n\024PixelClockIn"
    "foBuffer\022\022\n\nchannel_id\030\001 \001(\005\022\022\n\ntime_sta"
    "mp\030\002 \001(\003\022\021\n\tdirection\030\003 \002(\005", 107);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "pixel_clock_info.proto", &protobuf_RegisterTypes);
  PixelClockInfoBuffer::default_instance_ = new PixelClockInfoBuffer();
  PixelClockInfoBuffer::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_pixel_5fclock_5finfo_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_pixel_5fclock_5finfo_2eproto {
  StaticDescriptorInitializer_pixel_5fclock_5finfo_2eproto() {
    protobuf_AddDesc_pixel_5fclock_5finfo_2eproto();
  }
} static_descriptor_initializer_pixel_5fclock_5finfo_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int PixelClockInfoBuffer::kChannelIdFieldNumber;
const int PixelClockInfoBuffer::kTimeStampFieldNumber;
const int PixelClockInfoBuffer::kDirectionFieldNumber;
#endif  // !_MSC_VER

PixelClockInfoBuffer::PixelClockInfoBuffer()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void PixelClockInfoBuffer::InitAsDefaultInstance() {
}

PixelClockInfoBuffer::PixelClockInfoBuffer(const PixelClockInfoBuffer& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void PixelClockInfoBuffer::SharedCtor() {
  _cached_size_ = 0;
  channel_id_ = 0;
  time_stamp_ = GOOGLE_LONGLONG(0);
  direction_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PixelClockInfoBuffer::~PixelClockInfoBuffer() {
  SharedDtor();
}

void PixelClockInfoBuffer::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PixelClockInfoBuffer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PixelClockInfoBuffer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PixelClockInfoBuffer_descriptor_;
}

const PixelClockInfoBuffer& PixelClockInfoBuffer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_pixel_5fclock_5finfo_2eproto();
  return *default_instance_;
}

PixelClockInfoBuffer* PixelClockInfoBuffer::default_instance_ = NULL;

PixelClockInfoBuffer* PixelClockInfoBuffer::New() const {
  return new PixelClockInfoBuffer;
}

void PixelClockInfoBuffer::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    channel_id_ = 0;
    time_stamp_ = GOOGLE_LONGLONG(0);
    direction_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PixelClockInfoBuffer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 channel_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &channel_id_)));
          set_has_channel_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_time_stamp;
        break;
      }

      // optional int64 time_stamp = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_time_stamp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &time_stamp_)));
          set_has_time_stamp();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_direction;
        break;
      }

      // required int32 direction = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_direction:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &direction_)));
          set_has_direction();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void PixelClockInfoBuffer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 channel_id = 1;
  if (has_channel_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->channel_id(), output);
  }

  // optional int64 time_stamp = 2;
  if (has_time_stamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->time_stamp(), output);
  }

  // required int32 direction = 3;
  if (has_direction()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->direction(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* PixelClockInfoBuffer::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 channel_id = 1;
  if (has_channel_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->channel_id(), target);
  }

  // optional int64 time_stamp = 2;
  if (has_time_stamp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->time_stamp(), target);
  }

  // required int32 direction = 3;
  if (has_direction()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->direction(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int PixelClockInfoBuffer::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 channel_id = 1;
    if (has_channel_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->channel_id());
    }

    // optional int64 time_stamp = 2;
    if (has_time_stamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->time_stamp());
    }

    // required int32 direction = 3;
    if (has_direction()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->direction());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PixelClockInfoBuffer::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PixelClockInfoBuffer* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PixelClockInfoBuffer*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PixelClockInfoBuffer::MergeFrom(const PixelClockInfoBuffer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_channel_id()) {
      set_channel_id(from.channel_id());
    }
    if (from.has_time_stamp()) {
      set_time_stamp(from.time_stamp());
    }
    if (from.has_direction()) {
      set_direction(from.direction());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PixelClockInfoBuffer::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PixelClockInfoBuffer::CopyFrom(const PixelClockInfoBuffer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PixelClockInfoBuffer::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000004) != 0x00000004) return false;

  return true;
}

void PixelClockInfoBuffer::Swap(PixelClockInfoBuffer* other) {
  if (other != this) {
    std::swap(channel_id_, other->channel_id_);
    std::swap(time_stamp_, other->time_stamp_);
    std::swap(direction_, other->direction_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PixelClockInfoBuffer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PixelClockInfoBuffer_descriptor_;
  metadata.reflection = PixelClockInfoBuffer_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
