// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: csclient/ParamDictionary.proto

#ifndef PROTOBUF_csclient_2fParamDictionary_2eproto__INCLUDED
#define PROTOBUF_csclient_2fParamDictionary_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace cms_8120 {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_csclient_2fParamDictionary_2eproto();
void protobuf_AssignDesc_csclient_2fParamDictionary_2eproto();
void protobuf_ShutdownFile_csclient_2fParamDictionary_2eproto();

class ParamDictionary;
class ReqParamDictionary;
class ParamDictionaryList;

// ===================================================================

class ParamDictionary : public ::google::protobuf::Message {
 public:
  ParamDictionary();
  virtual ~ParamDictionary();
  
  ParamDictionary(const ParamDictionary& from);
  
  inline ParamDictionary& operator=(const ParamDictionary& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ParamDictionary& default_instance();
  
  void Swap(ParamDictionary* other);
  
  // implements Message ----------------------------------------------
  
  ParamDictionary* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ParamDictionary& from);
  void MergeFrom(const ParamDictionary& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // optional string strCode = 2;
  inline bool has_strcode() const;
  inline void clear_strcode();
  static const int kStrCodeFieldNumber = 2;
  inline const ::std::string& strcode() const;
  inline void set_strcode(const ::std::string& value);
  inline void set_strcode(const char* value);
  inline void set_strcode(const char* value, size_t size);
  inline ::std::string* mutable_strcode();
  inline ::std::string* release_strcode();
  
  // optional string strName = 3;
  inline bool has_strname() const;
  inline void clear_strname();
  static const int kStrNameFieldNumber = 3;
  inline const ::std::string& strname() const;
  inline void set_strname(const ::std::string& value);
  inline void set_strname(const char* value);
  inline void set_strname(const char* value, size_t size);
  inline ::std::string* mutable_strname();
  inline ::std::string* release_strname();
  
  // optional int32 nsortId = 4;
  inline bool has_nsortid() const;
  inline void clear_nsortid();
  static const int kNsortIdFieldNumber = 4;
  inline ::google::protobuf::int32 nsortid() const;
  inline void set_nsortid(::google::protobuf::int32 value);
  
  // optional int32 enable = 5;
  inline bool has_enable() const;
  inline void clear_enable();
  static const int kEnableFieldNumber = 5;
  inline ::google::protobuf::int32 enable() const;
  inline void set_enable(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:cms_8120.ParamDictionary)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_strcode();
  inline void clear_has_strcode();
  inline void set_has_strname();
  inline void clear_has_strname();
  inline void set_has_nsortid();
  inline void clear_has_nsortid();
  inline void set_has_enable();
  inline void clear_has_enable();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* strcode_;
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 nsortid_;
  ::std::string* strname_;
  ::google::protobuf::int32 enable_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void  protobuf_AddDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_AssignDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_ShutdownFile_csclient_2fParamDictionary_2eproto();
  
  void InitAsDefaultInstance();
  static ParamDictionary* default_instance_;
};
// -------------------------------------------------------------------

class ReqParamDictionary : public ::google::protobuf::Message {
 public:
  ReqParamDictionary();
  virtual ~ReqParamDictionary();
  
  ReqParamDictionary(const ReqParamDictionary& from);
  
  inline ReqParamDictionary& operator=(const ReqParamDictionary& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqParamDictionary& default_instance();
  
  void Swap(ReqParamDictionary* other);
  
  // implements Message ----------------------------------------------
  
  ReqParamDictionary* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqParamDictionary& from);
  void MergeFrom(const ReqParamDictionary& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 cmd = 1;
  inline bool has_cmd() const;
  inline void clear_cmd();
  static const int kCmdFieldNumber = 1;
  inline ::google::protobuf::int32 cmd() const;
  inline void set_cmd(::google::protobuf::int32 value);
  
  // optional int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);
  
  // optional string strCode = 3;
  inline bool has_strcode() const;
  inline void clear_strcode();
  static const int kStrCodeFieldNumber = 3;
  inline const ::std::string& strcode() const;
  inline void set_strcode(const ::std::string& value);
  inline void set_strcode(const char* value);
  inline void set_strcode(const char* value, size_t size);
  inline ::std::string* mutable_strcode();
  inline ::std::string* release_strcode();
  
  // optional string strName = 4;
  inline bool has_strname() const;
  inline void clear_strname();
  static const int kStrNameFieldNumber = 4;
  inline const ::std::string& strname() const;
  inline void set_strname(const ::std::string& value);
  inline void set_strname(const char* value);
  inline void set_strname(const char* value, size_t size);
  inline ::std::string* mutable_strname();
  inline ::std::string* release_strname();
  
  // optional int32 nsortId = 5;
  inline bool has_nsortid() const;
  inline void clear_nsortid();
  static const int kNsortIdFieldNumber = 5;
  inline ::google::protobuf::int32 nsortid() const;
  inline void set_nsortid(::google::protobuf::int32 value);
  
  // optional int32 enable = 6;
  inline bool has_enable() const;
  inline void clear_enable();
  static const int kEnableFieldNumber = 6;
  inline ::google::protobuf::int32 enable() const;
  inline void set_enable(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:cms_8120.ReqParamDictionary)
 private:
  inline void set_has_cmd();
  inline void clear_has_cmd();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_strcode();
  inline void clear_has_strcode();
  inline void set_has_strname();
  inline void clear_has_strname();
  inline void set_has_nsortid();
  inline void clear_has_nsortid();
  inline void set_has_enable();
  inline void clear_has_enable();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 cmd_;
  ::google::protobuf::int32 id_;
  ::std::string* strcode_;
  ::std::string* strname_;
  ::google::protobuf::int32 nsortid_;
  ::google::protobuf::int32 enable_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(6 + 31) / 32];
  
  friend void  protobuf_AddDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_AssignDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_ShutdownFile_csclient_2fParamDictionary_2eproto();
  
  void InitAsDefaultInstance();
  static ReqParamDictionary* default_instance_;
};
// -------------------------------------------------------------------

class ParamDictionaryList : public ::google::protobuf::Message {
 public:
  ParamDictionaryList();
  virtual ~ParamDictionaryList();
  
  ParamDictionaryList(const ParamDictionaryList& from);
  
  inline ParamDictionaryList& operator=(const ParamDictionaryList& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ParamDictionaryList& default_instance();
  
  void Swap(ParamDictionaryList* other);
  
  // implements Message ----------------------------------------------
  
  ParamDictionaryList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ParamDictionaryList& from);
  void MergeFrom(const ParamDictionaryList& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 cmd = 1;
  inline bool has_cmd() const;
  inline void clear_cmd();
  static const int kCmdFieldNumber = 1;
  inline ::google::protobuf::int32 cmd() const;
  inline void set_cmd(::google::protobuf::int32 value);
  
  // optional int32 result = 2;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 2;
  inline ::google::protobuf::int32 result() const;
  inline void set_result(::google::protobuf::int32 value);
  
  // optional string result_info = 3;
  inline bool has_result_info() const;
  inline void clear_result_info();
  static const int kResultInfoFieldNumber = 3;
  inline const ::std::string& result_info() const;
  inline void set_result_info(const ::std::string& value);
  inline void set_result_info(const char* value);
  inline void set_result_info(const char* value, size_t size);
  inline ::std::string* mutable_result_info();
  inline ::std::string* release_result_info();
  
  // repeated .cms_8120.ParamDictionary paramDictionary = 4;
  inline int paramdictionary_size() const;
  inline void clear_paramdictionary();
  static const int kParamDictionaryFieldNumber = 4;
  inline const ::cms_8120::ParamDictionary& paramdictionary(int index) const;
  inline ::cms_8120::ParamDictionary* mutable_paramdictionary(int index);
  inline ::cms_8120::ParamDictionary* add_paramdictionary();
  inline const ::google::protobuf::RepeatedPtrField< ::cms_8120::ParamDictionary >&
      paramdictionary() const;
  inline ::google::protobuf::RepeatedPtrField< ::cms_8120::ParamDictionary >*
      mutable_paramdictionary();
  
  // @@protoc_insertion_point(class_scope:cms_8120.ParamDictionaryList)
 private:
  inline void set_has_cmd();
  inline void clear_has_cmd();
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_result_info();
  inline void clear_has_result_info();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 cmd_;
  ::google::protobuf::int32 result_;
  ::std::string* result_info_;
  ::google::protobuf::RepeatedPtrField< ::cms_8120::ParamDictionary > paramdictionary_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_AssignDesc_csclient_2fParamDictionary_2eproto();
  friend void protobuf_ShutdownFile_csclient_2fParamDictionary_2eproto();
  
  void InitAsDefaultInstance();
  static ParamDictionaryList* default_instance_;
};
// ===================================================================


// ===================================================================

// ParamDictionary

// optional int32 id = 1;
inline bool ParamDictionary::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ParamDictionary::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ParamDictionary::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ParamDictionary::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 ParamDictionary::id() const {
  return id_;
}
inline void ParamDictionary::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional string strCode = 2;
inline bool ParamDictionary::has_strcode() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ParamDictionary::set_has_strcode() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ParamDictionary::clear_has_strcode() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ParamDictionary::clear_strcode() {
  if (strcode_ != &::google::protobuf::internal::kEmptyString) {
    strcode_->clear();
  }
  clear_has_strcode();
}
inline const ::std::string& ParamDictionary::strcode() const {
  return *strcode_;
}
inline void ParamDictionary::set_strcode(const ::std::string& value) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(value);
}
inline void ParamDictionary::set_strcode(const char* value) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(value);
}
inline void ParamDictionary::set_strcode(const char* value, size_t size) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ParamDictionary::mutable_strcode() {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  return strcode_;
}
inline ::std::string* ParamDictionary::release_strcode() {
  clear_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = strcode_;
    strcode_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string strName = 3;
inline bool ParamDictionary::has_strname() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ParamDictionary::set_has_strname() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ParamDictionary::clear_has_strname() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ParamDictionary::clear_strname() {
  if (strname_ != &::google::protobuf::internal::kEmptyString) {
    strname_->clear();
  }
  clear_has_strname();
}
inline const ::std::string& ParamDictionary::strname() const {
  return *strname_;
}
inline void ParamDictionary::set_strname(const ::std::string& value) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(value);
}
inline void ParamDictionary::set_strname(const char* value) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(value);
}
inline void ParamDictionary::set_strname(const char* value, size_t size) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ParamDictionary::mutable_strname() {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  return strname_;
}
inline ::std::string* ParamDictionary::release_strname() {
  clear_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = strname_;
    strname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 nsortId = 4;
inline bool ParamDictionary::has_nsortid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ParamDictionary::set_has_nsortid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ParamDictionary::clear_has_nsortid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ParamDictionary::clear_nsortid() {
  nsortid_ = 0;
  clear_has_nsortid();
}
inline ::google::protobuf::int32 ParamDictionary::nsortid() const {
  return nsortid_;
}
inline void ParamDictionary::set_nsortid(::google::protobuf::int32 value) {
  set_has_nsortid();
  nsortid_ = value;
}

// optional int32 enable = 5;
inline bool ParamDictionary::has_enable() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ParamDictionary::set_has_enable() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ParamDictionary::clear_has_enable() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ParamDictionary::clear_enable() {
  enable_ = 0;
  clear_has_enable();
}
inline ::google::protobuf::int32 ParamDictionary::enable() const {
  return enable_;
}
inline void ParamDictionary::set_enable(::google::protobuf::int32 value) {
  set_has_enable();
  enable_ = value;
}

// -------------------------------------------------------------------

// ReqParamDictionary

// optional int32 cmd = 1;
inline bool ReqParamDictionary::has_cmd() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqParamDictionary::set_has_cmd() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqParamDictionary::clear_has_cmd() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqParamDictionary::clear_cmd() {
  cmd_ = 0;
  clear_has_cmd();
}
inline ::google::protobuf::int32 ReqParamDictionary::cmd() const {
  return cmd_;
}
inline void ReqParamDictionary::set_cmd(::google::protobuf::int32 value) {
  set_has_cmd();
  cmd_ = value;
}

// optional int32 id = 2;
inline bool ReqParamDictionary::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqParamDictionary::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqParamDictionary::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqParamDictionary::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 ReqParamDictionary::id() const {
  return id_;
}
inline void ReqParamDictionary::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional string strCode = 3;
inline bool ReqParamDictionary::has_strcode() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqParamDictionary::set_has_strcode() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqParamDictionary::clear_has_strcode() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqParamDictionary::clear_strcode() {
  if (strcode_ != &::google::protobuf::internal::kEmptyString) {
    strcode_->clear();
  }
  clear_has_strcode();
}
inline const ::std::string& ReqParamDictionary::strcode() const {
  return *strcode_;
}
inline void ReqParamDictionary::set_strcode(const ::std::string& value) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(value);
}
inline void ReqParamDictionary::set_strcode(const char* value) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(value);
}
inline void ReqParamDictionary::set_strcode(const char* value, size_t size) {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  strcode_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqParamDictionary::mutable_strcode() {
  set_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    strcode_ = new ::std::string;
  }
  return strcode_;
}
inline ::std::string* ReqParamDictionary::release_strcode() {
  clear_has_strcode();
  if (strcode_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = strcode_;
    strcode_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string strName = 4;
inline bool ReqParamDictionary::has_strname() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqParamDictionary::set_has_strname() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqParamDictionary::clear_has_strname() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqParamDictionary::clear_strname() {
  if (strname_ != &::google::protobuf::internal::kEmptyString) {
    strname_->clear();
  }
  clear_has_strname();
}
inline const ::std::string& ReqParamDictionary::strname() const {
  return *strname_;
}
inline void ReqParamDictionary::set_strname(const ::std::string& value) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(value);
}
inline void ReqParamDictionary::set_strname(const char* value) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(value);
}
inline void ReqParamDictionary::set_strname(const char* value, size_t size) {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  strname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqParamDictionary::mutable_strname() {
  set_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    strname_ = new ::std::string;
  }
  return strname_;
}
inline ::std::string* ReqParamDictionary::release_strname() {
  clear_has_strname();
  if (strname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = strname_;
    strname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional int32 nsortId = 5;
inline bool ReqParamDictionary::has_nsortid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ReqParamDictionary::set_has_nsortid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ReqParamDictionary::clear_has_nsortid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ReqParamDictionary::clear_nsortid() {
  nsortid_ = 0;
  clear_has_nsortid();
}
inline ::google::protobuf::int32 ReqParamDictionary::nsortid() const {
  return nsortid_;
}
inline void ReqParamDictionary::set_nsortid(::google::protobuf::int32 value) {
  set_has_nsortid();
  nsortid_ = value;
}

// optional int32 enable = 6;
inline bool ReqParamDictionary::has_enable() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void ReqParamDictionary::set_has_enable() {
  _has_bits_[0] |= 0x00000020u;
}
inline void ReqParamDictionary::clear_has_enable() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void ReqParamDictionary::clear_enable() {
  enable_ = 0;
  clear_has_enable();
}
inline ::google::protobuf::int32 ReqParamDictionary::enable() const {
  return enable_;
}
inline void ReqParamDictionary::set_enable(::google::protobuf::int32 value) {
  set_has_enable();
  enable_ = value;
}

// -------------------------------------------------------------------

// ParamDictionaryList

// optional int32 cmd = 1;
inline bool ParamDictionaryList::has_cmd() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ParamDictionaryList::set_has_cmd() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ParamDictionaryList::clear_has_cmd() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ParamDictionaryList::clear_cmd() {
  cmd_ = 0;
  clear_has_cmd();
}
inline ::google::protobuf::int32 ParamDictionaryList::cmd() const {
  return cmd_;
}
inline void ParamDictionaryList::set_cmd(::google::protobuf::int32 value) {
  set_has_cmd();
  cmd_ = value;
}

// optional int32 result = 2;
inline bool ParamDictionaryList::has_result() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ParamDictionaryList::set_has_result() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ParamDictionaryList::clear_has_result() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ParamDictionaryList::clear_result() {
  result_ = 0;
  clear_has_result();
}
inline ::google::protobuf::int32 ParamDictionaryList::result() const {
  return result_;
}
inline void ParamDictionaryList::set_result(::google::protobuf::int32 value) {
  set_has_result();
  result_ = value;
}

// optional string result_info = 3;
inline bool ParamDictionaryList::has_result_info() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ParamDictionaryList::set_has_result_info() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ParamDictionaryList::clear_has_result_info() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ParamDictionaryList::clear_result_info() {
  if (result_info_ != &::google::protobuf::internal::kEmptyString) {
    result_info_->clear();
  }
  clear_has_result_info();
}
inline const ::std::string& ParamDictionaryList::result_info() const {
  return *result_info_;
}
inline void ParamDictionaryList::set_result_info(const ::std::string& value) {
  set_has_result_info();
  if (result_info_ == &::google::protobuf::internal::kEmptyString) {
    result_info_ = new ::std::string;
  }
  result_info_->assign(value);
}
inline void ParamDictionaryList::set_result_info(const char* value) {
  set_has_result_info();
  if (result_info_ == &::google::protobuf::internal::kEmptyString) {
    result_info_ = new ::std::string;
  }
  result_info_->assign(value);
}
inline void ParamDictionaryList::set_result_info(const char* value, size_t size) {
  set_has_result_info();
  if (result_info_ == &::google::protobuf::internal::kEmptyString) {
    result_info_ = new ::std::string;
  }
  result_info_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ParamDictionaryList::mutable_result_info() {
  set_has_result_info();
  if (result_info_ == &::google::protobuf::internal::kEmptyString) {
    result_info_ = new ::std::string;
  }
  return result_info_;
}
inline ::std::string* ParamDictionaryList::release_result_info() {
  clear_has_result_info();
  if (result_info_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = result_info_;
    result_info_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .cms_8120.ParamDictionary paramDictionary = 4;
inline int ParamDictionaryList::paramdictionary_size() const {
  return paramdictionary_.size();
}
inline void ParamDictionaryList::clear_paramdictionary() {
  paramdictionary_.Clear();
}
inline const ::cms_8120::ParamDictionary& ParamDictionaryList::paramdictionary(int index) const {
  return paramdictionary_.Get(index);
}
inline ::cms_8120::ParamDictionary* ParamDictionaryList::mutable_paramdictionary(int index) {
  return paramdictionary_.Mutable(index);
}
inline ::cms_8120::ParamDictionary* ParamDictionaryList::add_paramdictionary() {
  return paramdictionary_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::cms_8120::ParamDictionary >&
ParamDictionaryList::paramdictionary() const {
  return paramdictionary_;
}
inline ::google::protobuf::RepeatedPtrField< ::cms_8120::ParamDictionary >*
ParamDictionaryList::mutable_paramdictionary() {
  return &paramdictionary_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace cms_8120

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_csclient_2fParamDictionary_2eproto__INCLUDED
