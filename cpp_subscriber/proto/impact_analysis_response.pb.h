// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cpp_subscriber/proto/impact_analysis_response.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/timestamp.pb.h>
#include "config_change.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto;
class ImpactAnalysisResponse;
class ImpactAnalysisResponseDefaultTypeInternal;
extern ImpactAnalysisResponseDefaultTypeInternal _ImpactAnalysisResponse_default_instance_;
class QueueImpactAnalysis;
class QueueImpactAnalysisDefaultTypeInternal;
extern QueueImpactAnalysisDefaultTypeInternal _QueueImpactAnalysis_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::ImpactAnalysisResponse* Arena::CreateMaybeMessage<::ImpactAnalysisResponse>(Arena*);
template<> ::QueueImpactAnalysis* Arena::CreateMaybeMessage<::QueueImpactAnalysis>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class QueueImpactAnalysis :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:QueueImpactAnalysis) */ {
 public:
  QueueImpactAnalysis();
  virtual ~QueueImpactAnalysis();

  QueueImpactAnalysis(const QueueImpactAnalysis& from);
  QueueImpactAnalysis(QueueImpactAnalysis&& from) noexcept
    : QueueImpactAnalysis() {
    *this = ::std::move(from);
  }

  inline QueueImpactAnalysis& operator=(const QueueImpactAnalysis& from) {
    CopyFrom(from);
    return *this;
  }
  inline QueueImpactAnalysis& operator=(QueueImpactAnalysis&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const QueueImpactAnalysis& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const QueueImpactAnalysis* internal_default_instance() {
    return reinterpret_cast<const QueueImpactAnalysis*>(
               &_QueueImpactAnalysis_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(QueueImpactAnalysis& a, QueueImpactAnalysis& b) {
    a.Swap(&b);
  }
  inline void Swap(QueueImpactAnalysis* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline QueueImpactAnalysis* New() const final {
    return CreateMaybeMessage<QueueImpactAnalysis>(nullptr);
  }

  QueueImpactAnalysis* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<QueueImpactAnalysis>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const QueueImpactAnalysis& from);
  void MergeFrom(const QueueImpactAnalysis& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(QueueImpactAnalysis* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "QueueImpactAnalysis";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto);
    return ::descriptor_table_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kQueueIdFieldNumber = 1,
    kAnalysisPeriodStartFieldNumber = 7,
    kAnalysisPeriodEndFieldNumber = 8,
    kDesiredSLAMinFieldNumber = 2,
    kPreviousSLAMinFieldNumber = 3,
    kNewSLAMinFieldNumber = 4,
    kPreviousAvgVideoVolumePerHourFieldNumber = 5,
    kNewAvgVideoVolumePerHourFieldNumber = 6,
  };
  // string queue_id = 1;
  void clear_queue_id();
  const std::string& queue_id() const;
  void set_queue_id(const std::string& value);
  void set_queue_id(std::string&& value);
  void set_queue_id(const char* value);
  void set_queue_id(const char* value, size_t size);
  std::string* mutable_queue_id();
  std::string* release_queue_id();
  void set_allocated_queue_id(std::string* queue_id);
  private:
  const std::string& _internal_queue_id() const;
  void _internal_set_queue_id(const std::string& value);
  std::string* _internal_mutable_queue_id();
  public:

  // .google.protobuf.Timestamp analysis_period_start = 7;
  bool has_analysis_period_start() const;
  private:
  bool _internal_has_analysis_period_start() const;
  public:
  void clear_analysis_period_start();
  const PROTOBUF_NAMESPACE_ID::Timestamp& analysis_period_start() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* release_analysis_period_start();
  PROTOBUF_NAMESPACE_ID::Timestamp* mutable_analysis_period_start();
  void set_allocated_analysis_period_start(PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_start);
  private:
  const PROTOBUF_NAMESPACE_ID::Timestamp& _internal_analysis_period_start() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* _internal_mutable_analysis_period_start();
  public:

  // .google.protobuf.Timestamp analysis_period_end = 8;
  bool has_analysis_period_end() const;
  private:
  bool _internal_has_analysis_period_end() const;
  public:
  void clear_analysis_period_end();
  const PROTOBUF_NAMESPACE_ID::Timestamp& analysis_period_end() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* release_analysis_period_end();
  PROTOBUF_NAMESPACE_ID::Timestamp* mutable_analysis_period_end();
  void set_allocated_analysis_period_end(PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_end);
  private:
  const PROTOBUF_NAMESPACE_ID::Timestamp& _internal_analysis_period_end() const;
  PROTOBUF_NAMESPACE_ID::Timestamp* _internal_mutable_analysis_period_end();
  public:

  // int32 desired_SLA_min = 2;
  void clear_desired_sla_min();
  ::PROTOBUF_NAMESPACE_ID::int32 desired_sla_min() const;
  void set_desired_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_desired_sla_min() const;
  void _internal_set_desired_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 previous_SLA_min = 3;
  void clear_previous_sla_min();
  ::PROTOBUF_NAMESPACE_ID::int32 previous_sla_min() const;
  void set_previous_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_previous_sla_min() const;
  void _internal_set_previous_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 new_SLA_min = 4;
  void clear_new_sla_min();
  ::PROTOBUF_NAMESPACE_ID::int32 new_sla_min() const;
  void set_new_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_new_sla_min() const;
  void _internal_set_new_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 previous_avg_video_volume_per_hour = 5;
  void clear_previous_avg_video_volume_per_hour();
  ::PROTOBUF_NAMESPACE_ID::int32 previous_avg_video_volume_per_hour() const;
  void set_previous_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_previous_avg_video_volume_per_hour() const;
  void _internal_set_previous_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 new_avg_video_volume_per_hour = 6;
  void clear_new_avg_video_volume_per_hour();
  ::PROTOBUF_NAMESPACE_ID::int32 new_avg_video_volume_per_hour() const;
  void set_new_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_new_avg_video_volume_per_hour() const;
  void _internal_set_new_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:QueueImpactAnalysis)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr queue_id_;
  PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_start_;
  PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_end_;
  ::PROTOBUF_NAMESPACE_ID::int32 desired_sla_min_;
  ::PROTOBUF_NAMESPACE_ID::int32 previous_sla_min_;
  ::PROTOBUF_NAMESPACE_ID::int32 new_sla_min_;
  ::PROTOBUF_NAMESPACE_ID::int32 previous_avg_video_volume_per_hour_;
  ::PROTOBUF_NAMESPACE_ID::int32 new_avg_video_volume_per_hour_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto;
};
// -------------------------------------------------------------------

class ImpactAnalysisResponse :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ImpactAnalysisResponse) */ {
 public:
  ImpactAnalysisResponse();
  virtual ~ImpactAnalysisResponse();

  ImpactAnalysisResponse(const ImpactAnalysisResponse& from);
  ImpactAnalysisResponse(ImpactAnalysisResponse&& from) noexcept
    : ImpactAnalysisResponse() {
    *this = ::std::move(from);
  }

  inline ImpactAnalysisResponse& operator=(const ImpactAnalysisResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline ImpactAnalysisResponse& operator=(ImpactAnalysisResponse&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ImpactAnalysisResponse& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ImpactAnalysisResponse* internal_default_instance() {
    return reinterpret_cast<const ImpactAnalysisResponse*>(
               &_ImpactAnalysisResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ImpactAnalysisResponse& a, ImpactAnalysisResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(ImpactAnalysisResponse* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ImpactAnalysisResponse* New() const final {
    return CreateMaybeMessage<ImpactAnalysisResponse>(nullptr);
  }

  ImpactAnalysisResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ImpactAnalysisResponse>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ImpactAnalysisResponse& from);
  void MergeFrom(const ImpactAnalysisResponse& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ImpactAnalysisResponse* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ImpactAnalysisResponse";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto);
    return ::descriptor_table_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kQueueImpactAnalysisListFieldNumber = 2,
    kErrorMessageFieldNumber = 1,
    kRequestFieldNumber = 3,
  };
  // repeated .QueueImpactAnalysis queue_Impact_analysis_list = 2;
  int queue_impact_analysis_list_size() const;
  private:
  int _internal_queue_impact_analysis_list_size() const;
  public:
  void clear_queue_impact_analysis_list();
  ::QueueImpactAnalysis* mutable_queue_impact_analysis_list(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::QueueImpactAnalysis >*
      mutable_queue_impact_analysis_list();
  private:
  const ::QueueImpactAnalysis& _internal_queue_impact_analysis_list(int index) const;
  ::QueueImpactAnalysis* _internal_add_queue_impact_analysis_list();
  public:
  const ::QueueImpactAnalysis& queue_impact_analysis_list(int index) const;
  ::QueueImpactAnalysis* add_queue_impact_analysis_list();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::QueueImpactAnalysis >&
      queue_impact_analysis_list() const;

  // string error_message = 1;
  void clear_error_message();
  const std::string& error_message() const;
  void set_error_message(const std::string& value);
  void set_error_message(std::string&& value);
  void set_error_message(const char* value);
  void set_error_message(const char* value, size_t size);
  std::string* mutable_error_message();
  std::string* release_error_message();
  void set_allocated_error_message(std::string* error_message);
  private:
  const std::string& _internal_error_message() const;
  void _internal_set_error_message(const std::string& value);
  std::string* _internal_mutable_error_message();
  public:

  // .ConfigChangeRequest request = 3;
  bool has_request() const;
  private:
  bool _internal_has_request() const;
  public:
  void clear_request();
  const ::ConfigChangeRequest& request() const;
  ::ConfigChangeRequest* release_request();
  ::ConfigChangeRequest* mutable_request();
  void set_allocated_request(::ConfigChangeRequest* request);
  private:
  const ::ConfigChangeRequest& _internal_request() const;
  ::ConfigChangeRequest* _internal_mutable_request();
  public:

  // @@protoc_insertion_point(class_scope:ImpactAnalysisResponse)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::QueueImpactAnalysis > queue_impact_analysis_list_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr error_message_;
  ::ConfigChangeRequest* request_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// QueueImpactAnalysis

// string queue_id = 1;
inline void QueueImpactAnalysis::clear_queue_id() {
  queue_id_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& QueueImpactAnalysis::queue_id() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.queue_id)
  return _internal_queue_id();
}
inline void QueueImpactAnalysis::set_queue_id(const std::string& value) {
  _internal_set_queue_id(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.queue_id)
}
inline std::string* QueueImpactAnalysis::mutable_queue_id() {
  // @@protoc_insertion_point(field_mutable:QueueImpactAnalysis.queue_id)
  return _internal_mutable_queue_id();
}
inline const std::string& QueueImpactAnalysis::_internal_queue_id() const {
  return queue_id_.GetNoArena();
}
inline void QueueImpactAnalysis::_internal_set_queue_id(const std::string& value) {
  
  queue_id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void QueueImpactAnalysis::set_queue_id(std::string&& value) {
  
  queue_id_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:QueueImpactAnalysis.queue_id)
}
inline void QueueImpactAnalysis::set_queue_id(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  queue_id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:QueueImpactAnalysis.queue_id)
}
inline void QueueImpactAnalysis::set_queue_id(const char* value, size_t size) {
  
  queue_id_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:QueueImpactAnalysis.queue_id)
}
inline std::string* QueueImpactAnalysis::_internal_mutable_queue_id() {
  
  return queue_id_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* QueueImpactAnalysis::release_queue_id() {
  // @@protoc_insertion_point(field_release:QueueImpactAnalysis.queue_id)
  
  return queue_id_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void QueueImpactAnalysis::set_allocated_queue_id(std::string* queue_id) {
  if (queue_id != nullptr) {
    
  } else {
    
  }
  queue_id_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), queue_id);
  // @@protoc_insertion_point(field_set_allocated:QueueImpactAnalysis.queue_id)
}

// int32 desired_SLA_min = 2;
inline void QueueImpactAnalysis::clear_desired_sla_min() {
  desired_sla_min_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::_internal_desired_sla_min() const {
  return desired_sla_min_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::desired_sla_min() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.desired_SLA_min)
  return _internal_desired_sla_min();
}
inline void QueueImpactAnalysis::_internal_set_desired_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  desired_sla_min_ = value;
}
inline void QueueImpactAnalysis::set_desired_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_desired_sla_min(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.desired_SLA_min)
}

// int32 previous_SLA_min = 3;
inline void QueueImpactAnalysis::clear_previous_sla_min() {
  previous_sla_min_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::_internal_previous_sla_min() const {
  return previous_sla_min_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::previous_sla_min() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.previous_SLA_min)
  return _internal_previous_sla_min();
}
inline void QueueImpactAnalysis::_internal_set_previous_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  previous_sla_min_ = value;
}
inline void QueueImpactAnalysis::set_previous_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_previous_sla_min(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.previous_SLA_min)
}

// int32 new_SLA_min = 4;
inline void QueueImpactAnalysis::clear_new_sla_min() {
  new_sla_min_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::_internal_new_sla_min() const {
  return new_sla_min_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::new_sla_min() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.new_SLA_min)
  return _internal_new_sla_min();
}
inline void QueueImpactAnalysis::_internal_set_new_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  new_sla_min_ = value;
}
inline void QueueImpactAnalysis::set_new_sla_min(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_new_sla_min(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.new_SLA_min)
}

// int32 previous_avg_video_volume_per_hour = 5;
inline void QueueImpactAnalysis::clear_previous_avg_video_volume_per_hour() {
  previous_avg_video_volume_per_hour_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::_internal_previous_avg_video_volume_per_hour() const {
  return previous_avg_video_volume_per_hour_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::previous_avg_video_volume_per_hour() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.previous_avg_video_volume_per_hour)
  return _internal_previous_avg_video_volume_per_hour();
}
inline void QueueImpactAnalysis::_internal_set_previous_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  previous_avg_video_volume_per_hour_ = value;
}
inline void QueueImpactAnalysis::set_previous_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_previous_avg_video_volume_per_hour(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.previous_avg_video_volume_per_hour)
}

// int32 new_avg_video_volume_per_hour = 6;
inline void QueueImpactAnalysis::clear_new_avg_video_volume_per_hour() {
  new_avg_video_volume_per_hour_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::_internal_new_avg_video_volume_per_hour() const {
  return new_avg_video_volume_per_hour_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 QueueImpactAnalysis::new_avg_video_volume_per_hour() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.new_avg_video_volume_per_hour)
  return _internal_new_avg_video_volume_per_hour();
}
inline void QueueImpactAnalysis::_internal_set_new_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  new_avg_video_volume_per_hour_ = value;
}
inline void QueueImpactAnalysis::set_new_avg_video_volume_per_hour(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_new_avg_video_volume_per_hour(value);
  // @@protoc_insertion_point(field_set:QueueImpactAnalysis.new_avg_video_volume_per_hour)
}

// .google.protobuf.Timestamp analysis_period_start = 7;
inline bool QueueImpactAnalysis::_internal_has_analysis_period_start() const {
  return this != internal_default_instance() && analysis_period_start_ != nullptr;
}
inline bool QueueImpactAnalysis::has_analysis_period_start() const {
  return _internal_has_analysis_period_start();
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& QueueImpactAnalysis::_internal_analysis_period_start() const {
  const PROTOBUF_NAMESPACE_ID::Timestamp* p = analysis_period_start_;
  return p != nullptr ? *p : *reinterpret_cast<const PROTOBUF_NAMESPACE_ID::Timestamp*>(
      &PROTOBUF_NAMESPACE_ID::_Timestamp_default_instance_);
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& QueueImpactAnalysis::analysis_period_start() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.analysis_period_start)
  return _internal_analysis_period_start();
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::release_analysis_period_start() {
  // @@protoc_insertion_point(field_release:QueueImpactAnalysis.analysis_period_start)
  
  PROTOBUF_NAMESPACE_ID::Timestamp* temp = analysis_period_start_;
  analysis_period_start_ = nullptr;
  return temp;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::_internal_mutable_analysis_period_start() {
  
  if (analysis_period_start_ == nullptr) {
    auto* p = CreateMaybeMessage<PROTOBUF_NAMESPACE_ID::Timestamp>(GetArenaNoVirtual());
    analysis_period_start_ = p;
  }
  return analysis_period_start_;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::mutable_analysis_period_start() {
  // @@protoc_insertion_point(field_mutable:QueueImpactAnalysis.analysis_period_start)
  return _internal_mutable_analysis_period_start();
}
inline void QueueImpactAnalysis::set_allocated_analysis_period_start(PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_start) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(analysis_period_start_);
  }
  if (analysis_period_start) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(analysis_period_start)->GetArena();
    if (message_arena != submessage_arena) {
      analysis_period_start = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, analysis_period_start, submessage_arena);
    }
    
  } else {
    
  }
  analysis_period_start_ = analysis_period_start;
  // @@protoc_insertion_point(field_set_allocated:QueueImpactAnalysis.analysis_period_start)
}

// .google.protobuf.Timestamp analysis_period_end = 8;
inline bool QueueImpactAnalysis::_internal_has_analysis_period_end() const {
  return this != internal_default_instance() && analysis_period_end_ != nullptr;
}
inline bool QueueImpactAnalysis::has_analysis_period_end() const {
  return _internal_has_analysis_period_end();
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& QueueImpactAnalysis::_internal_analysis_period_end() const {
  const PROTOBUF_NAMESPACE_ID::Timestamp* p = analysis_period_end_;
  return p != nullptr ? *p : *reinterpret_cast<const PROTOBUF_NAMESPACE_ID::Timestamp*>(
      &PROTOBUF_NAMESPACE_ID::_Timestamp_default_instance_);
}
inline const PROTOBUF_NAMESPACE_ID::Timestamp& QueueImpactAnalysis::analysis_period_end() const {
  // @@protoc_insertion_point(field_get:QueueImpactAnalysis.analysis_period_end)
  return _internal_analysis_period_end();
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::release_analysis_period_end() {
  // @@protoc_insertion_point(field_release:QueueImpactAnalysis.analysis_period_end)
  
  PROTOBUF_NAMESPACE_ID::Timestamp* temp = analysis_period_end_;
  analysis_period_end_ = nullptr;
  return temp;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::_internal_mutable_analysis_period_end() {
  
  if (analysis_period_end_ == nullptr) {
    auto* p = CreateMaybeMessage<PROTOBUF_NAMESPACE_ID::Timestamp>(GetArenaNoVirtual());
    analysis_period_end_ = p;
  }
  return analysis_period_end_;
}
inline PROTOBUF_NAMESPACE_ID::Timestamp* QueueImpactAnalysis::mutable_analysis_period_end() {
  // @@protoc_insertion_point(field_mutable:QueueImpactAnalysis.analysis_period_end)
  return _internal_mutable_analysis_period_end();
}
inline void QueueImpactAnalysis::set_allocated_analysis_period_end(PROTOBUF_NAMESPACE_ID::Timestamp* analysis_period_end) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(analysis_period_end_);
  }
  if (analysis_period_end) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(analysis_period_end)->GetArena();
    if (message_arena != submessage_arena) {
      analysis_period_end = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, analysis_period_end, submessage_arena);
    }
    
  } else {
    
  }
  analysis_period_end_ = analysis_period_end;
  // @@protoc_insertion_point(field_set_allocated:QueueImpactAnalysis.analysis_period_end)
}

// -------------------------------------------------------------------

// ImpactAnalysisResponse

// string error_message = 1;
inline void ImpactAnalysisResponse::clear_error_message() {
  error_message_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& ImpactAnalysisResponse::error_message() const {
  // @@protoc_insertion_point(field_get:ImpactAnalysisResponse.error_message)
  return _internal_error_message();
}
inline void ImpactAnalysisResponse::set_error_message(const std::string& value) {
  _internal_set_error_message(value);
  // @@protoc_insertion_point(field_set:ImpactAnalysisResponse.error_message)
}
inline std::string* ImpactAnalysisResponse::mutable_error_message() {
  // @@protoc_insertion_point(field_mutable:ImpactAnalysisResponse.error_message)
  return _internal_mutable_error_message();
}
inline const std::string& ImpactAnalysisResponse::_internal_error_message() const {
  return error_message_.GetNoArena();
}
inline void ImpactAnalysisResponse::_internal_set_error_message(const std::string& value) {
  
  error_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void ImpactAnalysisResponse::set_error_message(std::string&& value) {
  
  error_message_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ImpactAnalysisResponse.error_message)
}
inline void ImpactAnalysisResponse::set_error_message(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  error_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ImpactAnalysisResponse.error_message)
}
inline void ImpactAnalysisResponse::set_error_message(const char* value, size_t size) {
  
  error_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ImpactAnalysisResponse.error_message)
}
inline std::string* ImpactAnalysisResponse::_internal_mutable_error_message() {
  
  return error_message_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* ImpactAnalysisResponse::release_error_message() {
  // @@protoc_insertion_point(field_release:ImpactAnalysisResponse.error_message)
  
  return error_message_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void ImpactAnalysisResponse::set_allocated_error_message(std::string* error_message) {
  if (error_message != nullptr) {
    
  } else {
    
  }
  error_message_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), error_message);
  // @@protoc_insertion_point(field_set_allocated:ImpactAnalysisResponse.error_message)
}

// repeated .QueueImpactAnalysis queue_Impact_analysis_list = 2;
inline int ImpactAnalysisResponse::_internal_queue_impact_analysis_list_size() const {
  return queue_impact_analysis_list_.size();
}
inline int ImpactAnalysisResponse::queue_impact_analysis_list_size() const {
  return _internal_queue_impact_analysis_list_size();
}
inline void ImpactAnalysisResponse::clear_queue_impact_analysis_list() {
  queue_impact_analysis_list_.Clear();
}
inline ::QueueImpactAnalysis* ImpactAnalysisResponse::mutable_queue_impact_analysis_list(int index) {
  // @@protoc_insertion_point(field_mutable:ImpactAnalysisResponse.queue_Impact_analysis_list)
  return queue_impact_analysis_list_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::QueueImpactAnalysis >*
ImpactAnalysisResponse::mutable_queue_impact_analysis_list() {
  // @@protoc_insertion_point(field_mutable_list:ImpactAnalysisResponse.queue_Impact_analysis_list)
  return &queue_impact_analysis_list_;
}
inline const ::QueueImpactAnalysis& ImpactAnalysisResponse::_internal_queue_impact_analysis_list(int index) const {
  return queue_impact_analysis_list_.Get(index);
}
inline const ::QueueImpactAnalysis& ImpactAnalysisResponse::queue_impact_analysis_list(int index) const {
  // @@protoc_insertion_point(field_get:ImpactAnalysisResponse.queue_Impact_analysis_list)
  return _internal_queue_impact_analysis_list(index);
}
inline ::QueueImpactAnalysis* ImpactAnalysisResponse::_internal_add_queue_impact_analysis_list() {
  return queue_impact_analysis_list_.Add();
}
inline ::QueueImpactAnalysis* ImpactAnalysisResponse::add_queue_impact_analysis_list() {
  // @@protoc_insertion_point(field_add:ImpactAnalysisResponse.queue_Impact_analysis_list)
  return _internal_add_queue_impact_analysis_list();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::QueueImpactAnalysis >&
ImpactAnalysisResponse::queue_impact_analysis_list() const {
  // @@protoc_insertion_point(field_list:ImpactAnalysisResponse.queue_Impact_analysis_list)
  return queue_impact_analysis_list_;
}

// .ConfigChangeRequest request = 3;
inline bool ImpactAnalysisResponse::_internal_has_request() const {
  return this != internal_default_instance() && request_ != nullptr;
}
inline bool ImpactAnalysisResponse::has_request() const {
  return _internal_has_request();
}
inline const ::ConfigChangeRequest& ImpactAnalysisResponse::_internal_request() const {
  const ::ConfigChangeRequest* p = request_;
  return p != nullptr ? *p : *reinterpret_cast<const ::ConfigChangeRequest*>(
      &::_ConfigChangeRequest_default_instance_);
}
inline const ::ConfigChangeRequest& ImpactAnalysisResponse::request() const {
  // @@protoc_insertion_point(field_get:ImpactAnalysisResponse.request)
  return _internal_request();
}
inline ::ConfigChangeRequest* ImpactAnalysisResponse::release_request() {
  // @@protoc_insertion_point(field_release:ImpactAnalysisResponse.request)
  
  ::ConfigChangeRequest* temp = request_;
  request_ = nullptr;
  return temp;
}
inline ::ConfigChangeRequest* ImpactAnalysisResponse::_internal_mutable_request() {
  
  if (request_ == nullptr) {
    auto* p = CreateMaybeMessage<::ConfigChangeRequest>(GetArenaNoVirtual());
    request_ = p;
  }
  return request_;
}
inline ::ConfigChangeRequest* ImpactAnalysisResponse::mutable_request() {
  // @@protoc_insertion_point(field_mutable:ImpactAnalysisResponse.request)
  return _internal_mutable_request();
}
inline void ImpactAnalysisResponse::set_allocated_request(::ConfigChangeRequest* request) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(request_);
  }
  if (request) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      request = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, request, submessage_arena);
    }
    
  } else {
    
  }
  request_ = request;
  // @@protoc_insertion_point(field_set_allocated:ImpactAnalysisResponse.request)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_cpp_5fsubscriber_2fproto_2fimpact_5fanalysis_5fresponse_2eproto
