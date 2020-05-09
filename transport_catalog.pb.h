// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: transport_catalog.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_transport_5fcatalog_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_transport_5fcatalog_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_transport_5fcatalog_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_transport_5fcatalog_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_transport_5fcatalog_2eproto;
namespace serialization {
class BusInfo;
class BusInfoDefaultTypeInternal;
extern BusInfoDefaultTypeInternal _BusInfo_default_instance_;
class StopInfo;
class StopInfoDefaultTypeInternal;
extern StopInfoDefaultTypeInternal _StopInfo_default_instance_;
class TransportCatalog;
class TransportCatalogDefaultTypeInternal;
extern TransportCatalogDefaultTypeInternal _TransportCatalog_default_instance_;
}  // namespace serialization
PROTOBUF_NAMESPACE_OPEN
template<> ::serialization::BusInfo* Arena::CreateMaybeMessage<::serialization::BusInfo>(Arena*);
template<> ::serialization::StopInfo* Arena::CreateMaybeMessage<::serialization::StopInfo>(Arena*);
template<> ::serialization::TransportCatalog* Arena::CreateMaybeMessage<::serialization::TransportCatalog>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace serialization {

// ===================================================================

class StopInfo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:serialization.StopInfo) */ {
 public:
  StopInfo();
  virtual ~StopInfo();

  StopInfo(const StopInfo& from);
  StopInfo(StopInfo&& from) noexcept
    : StopInfo() {
    *this = ::std::move(from);
  }

  inline StopInfo& operator=(const StopInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline StopInfo& operator=(StopInfo&& from) noexcept {
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
  static const StopInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const StopInfo* internal_default_instance() {
    return reinterpret_cast<const StopInfo*>(
               &_StopInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(StopInfo& a, StopInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(StopInfo* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline StopInfo* New() const final {
    return CreateMaybeMessage<StopInfo>(nullptr);
  }

  StopInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<StopInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const StopInfo& from);
  void MergeFrom(const StopInfo& from);
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
  void InternalSwap(StopInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "serialization.StopInfo";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_transport_5fcatalog_2eproto);
    return ::descriptor_table_transport_5fcatalog_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBusesFieldNumber = 2,
    kNameFieldNumber = 1,
  };
  // repeated string buses = 2;
  int buses_size() const;
  private:
  int _internal_buses_size() const;
  public:
  void clear_buses();
  const std::string& buses(int index) const;
  std::string* mutable_buses(int index);
  void set_buses(int index, const std::string& value);
  void set_buses(int index, std::string&& value);
  void set_buses(int index, const char* value);
  void set_buses(int index, const char* value, size_t size);
  std::string* add_buses();
  void add_buses(const std::string& value);
  void add_buses(std::string&& value);
  void add_buses(const char* value);
  void add_buses(const char* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& buses() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_buses();
  private:
  const std::string& _internal_buses(int index) const;
  std::string* _internal_add_buses();
  public:

  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // @@protoc_insertion_point(class_scope:serialization.StopInfo)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> buses_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_transport_5fcatalog_2eproto;
};
// -------------------------------------------------------------------

class BusInfo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:serialization.BusInfo) */ {
 public:
  BusInfo();
  virtual ~BusInfo();

  BusInfo(const BusInfo& from);
  BusInfo(BusInfo&& from) noexcept
    : BusInfo() {
    *this = ::std::move(from);
  }

  inline BusInfo& operator=(const BusInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline BusInfo& operator=(BusInfo&& from) noexcept {
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
  static const BusInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const BusInfo* internal_default_instance() {
    return reinterpret_cast<const BusInfo*>(
               &_BusInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(BusInfo& a, BusInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(BusInfo* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline BusInfo* New() const final {
    return CreateMaybeMessage<BusInfo>(nullptr);
  }

  BusInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<BusInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const BusInfo& from);
  void MergeFrom(const BusInfo& from);
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
  void InternalSwap(BusInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "serialization.BusInfo";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_transport_5fcatalog_2eproto);
    return ::descriptor_table_transport_5fcatalog_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 1,
    kStopCountFieldNumber = 2,
    kUniqueStopCountFieldNumber = 3,
    kGeoRouteLengthFieldNumber = 5,
    kRoadRouteLengthFieldNumber = 4,
  };
  // string name = 1;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // uint64 stop_count = 2;
  void clear_stop_count();
  ::PROTOBUF_NAMESPACE_ID::uint64 stop_count() const;
  void set_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_stop_count() const;
  void _internal_set_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 unique_stop_count = 3;
  void clear_unique_stop_count();
  ::PROTOBUF_NAMESPACE_ID::uint64 unique_stop_count() const;
  void set_unique_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_unique_stop_count() const;
  void _internal_set_unique_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // double geo_route_length = 5;
  void clear_geo_route_length();
  double geo_route_length() const;
  void set_geo_route_length(double value);
  private:
  double _internal_geo_route_length() const;
  void _internal_set_geo_route_length(double value);
  public:

  // int32 road_route_length = 4;
  void clear_road_route_length();
  ::PROTOBUF_NAMESPACE_ID::int32 road_route_length() const;
  void set_road_route_length(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_road_route_length() const;
  void _internal_set_road_route_length(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:serialization.BusInfo)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::uint64 stop_count_;
  ::PROTOBUF_NAMESPACE_ID::uint64 unique_stop_count_;
  double geo_route_length_;
  ::PROTOBUF_NAMESPACE_ID::int32 road_route_length_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_transport_5fcatalog_2eproto;
};
// -------------------------------------------------------------------

class TransportCatalog :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:serialization.TransportCatalog) */ {
 public:
  TransportCatalog();
  virtual ~TransportCatalog();

  TransportCatalog(const TransportCatalog& from);
  TransportCatalog(TransportCatalog&& from) noexcept
    : TransportCatalog() {
    *this = ::std::move(from);
  }

  inline TransportCatalog& operator=(const TransportCatalog& from) {
    CopyFrom(from);
    return *this;
  }
  inline TransportCatalog& operator=(TransportCatalog&& from) noexcept {
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
  static const TransportCatalog& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TransportCatalog* internal_default_instance() {
    return reinterpret_cast<const TransportCatalog*>(
               &_TransportCatalog_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(TransportCatalog& a, TransportCatalog& b) {
    a.Swap(&b);
  }
  inline void Swap(TransportCatalog* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline TransportCatalog* New() const final {
    return CreateMaybeMessage<TransportCatalog>(nullptr);
  }

  TransportCatalog* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<TransportCatalog>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const TransportCatalog& from);
  void MergeFrom(const TransportCatalog& from);
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
  void InternalSwap(TransportCatalog* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "serialization.TransportCatalog";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_transport_5fcatalog_2eproto);
    return ::descriptor_table_transport_5fcatalog_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBusesFieldNumber = 1,
    kStopsFieldNumber = 2,
  };
  // repeated .serialization.BusInfo buses = 1;
  int buses_size() const;
  private:
  int _internal_buses_size() const;
  public:
  void clear_buses();
  ::serialization::BusInfo* mutable_buses(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::BusInfo >*
      mutable_buses();
  private:
  const ::serialization::BusInfo& _internal_buses(int index) const;
  ::serialization::BusInfo* _internal_add_buses();
  public:
  const ::serialization::BusInfo& buses(int index) const;
  ::serialization::BusInfo* add_buses();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::BusInfo >&
      buses() const;

  // repeated .serialization.StopInfo stops = 2;
  int stops_size() const;
  private:
  int _internal_stops_size() const;
  public:
  void clear_stops();
  ::serialization::StopInfo* mutable_stops(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::StopInfo >*
      mutable_stops();
  private:
  const ::serialization::StopInfo& _internal_stops(int index) const;
  ::serialization::StopInfo* _internal_add_stops();
  public:
  const ::serialization::StopInfo& stops(int index) const;
  ::serialization::StopInfo* add_stops();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::StopInfo >&
      stops() const;

  // @@protoc_insertion_point(class_scope:serialization.TransportCatalog)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::BusInfo > buses_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::StopInfo > stops_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_transport_5fcatalog_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// StopInfo

// string name = 1;
inline void StopInfo::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& StopInfo::name() const {
  // @@protoc_insertion_point(field_get:serialization.StopInfo.name)
  return _internal_name();
}
inline void StopInfo::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:serialization.StopInfo.name)
}
inline std::string* StopInfo::mutable_name() {
  // @@protoc_insertion_point(field_mutable:serialization.StopInfo.name)
  return _internal_mutable_name();
}
inline const std::string& StopInfo::_internal_name() const {
  return name_.GetNoArena();
}
inline void StopInfo::_internal_set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void StopInfo::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:serialization.StopInfo.name)
}
inline void StopInfo::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:serialization.StopInfo.name)
}
inline void StopInfo::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:serialization.StopInfo.name)
}
inline std::string* StopInfo::_internal_mutable_name() {
  
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* StopInfo::release_name() {
  // @@protoc_insertion_point(field_release:serialization.StopInfo.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void StopInfo::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:serialization.StopInfo.name)
}

// repeated string buses = 2;
inline int StopInfo::_internal_buses_size() const {
  return buses_.size();
}
inline int StopInfo::buses_size() const {
  return _internal_buses_size();
}
inline void StopInfo::clear_buses() {
  buses_.Clear();
}
inline std::string* StopInfo::add_buses() {
  // @@protoc_insertion_point(field_add_mutable:serialization.StopInfo.buses)
  return _internal_add_buses();
}
inline const std::string& StopInfo::_internal_buses(int index) const {
  return buses_.Get(index);
}
inline const std::string& StopInfo::buses(int index) const {
  // @@protoc_insertion_point(field_get:serialization.StopInfo.buses)
  return _internal_buses(index);
}
inline std::string* StopInfo::mutable_buses(int index) {
  // @@protoc_insertion_point(field_mutable:serialization.StopInfo.buses)
  return buses_.Mutable(index);
}
inline void StopInfo::set_buses(int index, const std::string& value) {
  // @@protoc_insertion_point(field_set:serialization.StopInfo.buses)
  buses_.Mutable(index)->assign(value);
}
inline void StopInfo::set_buses(int index, std::string&& value) {
  // @@protoc_insertion_point(field_set:serialization.StopInfo.buses)
  buses_.Mutable(index)->assign(std::move(value));
}
inline void StopInfo::set_buses(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  buses_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:serialization.StopInfo.buses)
}
inline void StopInfo::set_buses(int index, const char* value, size_t size) {
  buses_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:serialization.StopInfo.buses)
}
inline std::string* StopInfo::_internal_add_buses() {
  return buses_.Add();
}
inline void StopInfo::add_buses(const std::string& value) {
  buses_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:serialization.StopInfo.buses)
}
inline void StopInfo::add_buses(std::string&& value) {
  buses_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:serialization.StopInfo.buses)
}
inline void StopInfo::add_buses(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  buses_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:serialization.StopInfo.buses)
}
inline void StopInfo::add_buses(const char* value, size_t size) {
  buses_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:serialization.StopInfo.buses)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
StopInfo::buses() const {
  // @@protoc_insertion_point(field_list:serialization.StopInfo.buses)
  return buses_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
StopInfo::mutable_buses() {
  // @@protoc_insertion_point(field_mutable_list:serialization.StopInfo.buses)
  return &buses_;
}

// -------------------------------------------------------------------

// BusInfo

// string name = 1;
inline void BusInfo::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& BusInfo::name() const {
  // @@protoc_insertion_point(field_get:serialization.BusInfo.name)
  return _internal_name();
}
inline void BusInfo::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:serialization.BusInfo.name)
}
inline std::string* BusInfo::mutable_name() {
  // @@protoc_insertion_point(field_mutable:serialization.BusInfo.name)
  return _internal_mutable_name();
}
inline const std::string& BusInfo::_internal_name() const {
  return name_.GetNoArena();
}
inline void BusInfo::_internal_set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void BusInfo::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:serialization.BusInfo.name)
}
inline void BusInfo::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:serialization.BusInfo.name)
}
inline void BusInfo::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:serialization.BusInfo.name)
}
inline std::string* BusInfo::_internal_mutable_name() {
  
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* BusInfo::release_name() {
  // @@protoc_insertion_point(field_release:serialization.BusInfo.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void BusInfo::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:serialization.BusInfo.name)
}

// uint64 stop_count = 2;
inline void BusInfo::clear_stop_count() {
  stop_count_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BusInfo::_internal_stop_count() const {
  return stop_count_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BusInfo::stop_count() const {
  // @@protoc_insertion_point(field_get:serialization.BusInfo.stop_count)
  return _internal_stop_count();
}
inline void BusInfo::_internal_set_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  stop_count_ = value;
}
inline void BusInfo::set_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_stop_count(value);
  // @@protoc_insertion_point(field_set:serialization.BusInfo.stop_count)
}

// uint64 unique_stop_count = 3;
inline void BusInfo::clear_unique_stop_count() {
  unique_stop_count_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BusInfo::_internal_unique_stop_count() const {
  return unique_stop_count_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BusInfo::unique_stop_count() const {
  // @@protoc_insertion_point(field_get:serialization.BusInfo.unique_stop_count)
  return _internal_unique_stop_count();
}
inline void BusInfo::_internal_set_unique_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  unique_stop_count_ = value;
}
inline void BusInfo::set_unique_stop_count(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_unique_stop_count(value);
  // @@protoc_insertion_point(field_set:serialization.BusInfo.unique_stop_count)
}

// int32 road_route_length = 4;
inline void BusInfo::clear_road_route_length() {
  road_route_length_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 BusInfo::_internal_road_route_length() const {
  return road_route_length_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 BusInfo::road_route_length() const {
  // @@protoc_insertion_point(field_get:serialization.BusInfo.road_route_length)
  return _internal_road_route_length();
}
inline void BusInfo::_internal_set_road_route_length(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  road_route_length_ = value;
}
inline void BusInfo::set_road_route_length(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_road_route_length(value);
  // @@protoc_insertion_point(field_set:serialization.BusInfo.road_route_length)
}

// double geo_route_length = 5;
inline void BusInfo::clear_geo_route_length() {
  geo_route_length_ = 0;
}
inline double BusInfo::_internal_geo_route_length() const {
  return geo_route_length_;
}
inline double BusInfo::geo_route_length() const {
  // @@protoc_insertion_point(field_get:serialization.BusInfo.geo_route_length)
  return _internal_geo_route_length();
}
inline void BusInfo::_internal_set_geo_route_length(double value) {
  
  geo_route_length_ = value;
}
inline void BusInfo::set_geo_route_length(double value) {
  _internal_set_geo_route_length(value);
  // @@protoc_insertion_point(field_set:serialization.BusInfo.geo_route_length)
}

// -------------------------------------------------------------------

// TransportCatalog

// repeated .serialization.BusInfo buses = 1;
inline int TransportCatalog::_internal_buses_size() const {
  return buses_.size();
}
inline int TransportCatalog::buses_size() const {
  return _internal_buses_size();
}
inline void TransportCatalog::clear_buses() {
  buses_.Clear();
}
inline ::serialization::BusInfo* TransportCatalog::mutable_buses(int index) {
  // @@protoc_insertion_point(field_mutable:serialization.TransportCatalog.buses)
  return buses_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::BusInfo >*
TransportCatalog::mutable_buses() {
  // @@protoc_insertion_point(field_mutable_list:serialization.TransportCatalog.buses)
  return &buses_;
}
inline const ::serialization::BusInfo& TransportCatalog::_internal_buses(int index) const {
  return buses_.Get(index);
}
inline const ::serialization::BusInfo& TransportCatalog::buses(int index) const {
  // @@protoc_insertion_point(field_get:serialization.TransportCatalog.buses)
  return _internal_buses(index);
}
inline ::serialization::BusInfo* TransportCatalog::_internal_add_buses() {
  return buses_.Add();
}
inline ::serialization::BusInfo* TransportCatalog::add_buses() {
  // @@protoc_insertion_point(field_add:serialization.TransportCatalog.buses)
  return _internal_add_buses();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::BusInfo >&
TransportCatalog::buses() const {
  // @@protoc_insertion_point(field_list:serialization.TransportCatalog.buses)
  return buses_;
}

// repeated .serialization.StopInfo stops = 2;
inline int TransportCatalog::_internal_stops_size() const {
  return stops_.size();
}
inline int TransportCatalog::stops_size() const {
  return _internal_stops_size();
}
inline void TransportCatalog::clear_stops() {
  stops_.Clear();
}
inline ::serialization::StopInfo* TransportCatalog::mutable_stops(int index) {
  // @@protoc_insertion_point(field_mutable:serialization.TransportCatalog.stops)
  return stops_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::StopInfo >*
TransportCatalog::mutable_stops() {
  // @@protoc_insertion_point(field_mutable_list:serialization.TransportCatalog.stops)
  return &stops_;
}
inline const ::serialization::StopInfo& TransportCatalog::_internal_stops(int index) const {
  return stops_.Get(index);
}
inline const ::serialization::StopInfo& TransportCatalog::stops(int index) const {
  // @@protoc_insertion_point(field_get:serialization.TransportCatalog.stops)
  return _internal_stops(index);
}
inline ::serialization::StopInfo* TransportCatalog::_internal_add_stops() {
  return stops_.Add();
}
inline ::serialization::StopInfo* TransportCatalog::add_stops() {
  // @@protoc_insertion_point(field_add:serialization.TransportCatalog.stops)
  return _internal_add_stops();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::serialization::StopInfo >&
TransportCatalog::stops() const {
  // @@protoc_insertion_point(field_list:serialization.TransportCatalog.stops)
  return stops_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace serialization

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_transport_5fcatalog_2eproto
