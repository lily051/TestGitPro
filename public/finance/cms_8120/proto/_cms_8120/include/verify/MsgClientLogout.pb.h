// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: verify/MsgClientLogout.proto

#ifndef PROTOBUF_verify_2fMsgClientLogout_2eproto__INCLUDED
#define PROTOBUF_verify_2fMsgClientLogout_2eproto__INCLUDED

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
#include "type/cmd.pb.h"
#include "type/ModuleType.pb.h"
// @@protoc_insertion_point(includes)

namespace cms_8120 {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_verify_2fMsgClientLogout_2eproto();
void protobuf_AssignDesc_verify_2fMsgClientLogout_2eproto();
void protobuf_ShutdownFile_verify_2fMsgClientLogout_2eproto();

class MsgClientLogout;

// ===================================================================

class MsgClientLogout : public ::google::protobuf::Message {
 public:
  MsgClientLogout();
  virtual ~MsgClientLogout();
  
  MsgClientLogout(const MsgClientLogout& from);
  
  inline MsgClientLogout& operator=(const MsgClientLogout& from) {
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
  static const MsgClientLogout& default_instance();
  
  void Swap(MsgClientLogout* other);
  
  // implements Message ----------------------------------------------
  
  MsgClientLogout* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MsgClientLogout& from);
  void MergeFrom(const MsgClientLogout& from);
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
  
  // optional int32 moudle_type = 2;
  inline bool has_moudle_type() const;
  inline void clear_moudle_type();
  static const int kMoudleTypeFieldNumber = 2;
  inline ::google::protobuf::int32 moudle_type() const;
  inline void set_moudle_type(::google::protobuf::int32 value);
  
  // optional string user_name = 3;
  inline bool has_user_name() const;
  inline void clear_user_name();
  static const int kUserNameFieldNumber = 3;
  inline const ::std::string& user_name() const;
  inline void set_user_name(const ::std::string& value);
  inline void set_user_name(const char* value);
  inline void set_user_name(const char* value, size_t size);
  inline ::std::string* mutable_user_name();
  inline ::std::string* release_user_name();
  
  // optional string ip = 4;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 4;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  
  // optional string mac = 5;
  inline bool has_mac() const;
  inline void clear_mac();
  static const int kMacFieldNumber = 5;
  inline const ::std::string& mac() const;
  inline void set_mac(const ::std::string& value);
  inline void set_mac(const char* value);
  inline void set_mac(const char* value, size_t size);
  inline ::std::string* mutable_mac();
  inline ::std::string* release_mac();
  
  // optional string client_version = 6;
  inline bool has_client_version() const;
  inline void clear_client_version();
  static const int kClientVersionFieldNumber = 6;
  inline const ::std::string& client_version() const;
  inline void set_client_version(const ::std::string& value);
  inline void set_client_version(const char* value);
  inline void set_client_version(const char* value, size_t size);
  inline ::std::string* mutable_client_version();
  inline ::std::string* release_client_version();
  
  // optional string dc_ip = 7;
  inline bool has_dc_ip() const;
  inline void clear_dc_ip();
  static const int kDcIpFieldNumber = 7;
  inline const ::std::string& dc_ip() const;
  inline void set_dc_ip(const ::std::string& value);
  inline void set_dc_ip(const char* value);
  inline void set_dc_ip(const char* value, size_t size);
  inline ::std::string* mutable_dc_ip();
  inline ::std::string* release_dc_ip();
  
  // @@protoc_insertion_point(class_scope:cms_8120.MsgClientLogout)
 private:
  inline void set_has_cmd();
  inline void clear_has_cmd();
  inline void set_has_moudle_type();
  inline void clear_has_moudle_type();
  inline void set_has_user_name();
  inline void clear_has_user_name();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_mac();
  inline void clear_has_mac();
  inline void set_has_client_version();
  inline void clear_has_client_version();
  inline void set_has_dc_ip();
  inline void clear_has_dc_ip();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::int32 cmd_;
  ::google::protobuf::int32 moudle_type_;
  ::std::string* user_name_;
  ::std::string* ip_;
  ::std::string* mac_;
  ::std::string* client_version_;
  ::std::string* dc_ip_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];
  
  friend void  protobuf_AddDesc_verify_2fMsgClientLogout_2eproto();
  friend void protobuf_AssignDesc_verify_2fMsgClientLogout_2eproto();
  friend void protobuf_ShutdownFile_verify_2fMsgClientLogout_2eproto();
  
  void InitAsDefaultInstance();
  static MsgClientLogout* default_instance_;
};
// ===================================================================


// ===================================================================

// MsgClientLogout

// optional int32 cmd = 1;
inline bool MsgClientLogout::has_cmd() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MsgClientLogout::set_has_cmd() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MsgClientLogout::clear_has_cmd() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MsgClientLogout::clear_cmd() {
  cmd_ = 0;
  clear_has_cmd();
}
inline ::google::protobuf::int32 MsgClientLogout::cmd() const {
  return cmd_;
}
inline void MsgClientLogout::set_cmd(::google::protobuf::int32 value) {
  set_has_cmd();
  cmd_ = value;
}

// optional int32 moudle_type = 2;
inline bool MsgClientLogout::has_moudle_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MsgClientLogout::set_has_moudle_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MsgClientLogout::clear_has_moudle_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MsgClientLogout::clear_moudle_type() {
  moudle_type_ = 0;
  clear_has_moudle_type();
}
inline ::google::protobuf::int32 MsgClientLogout::moudle_type() const {
  return moudle_type_;
}
inline void MsgClientLogout::set_moudle_type(::google::protobuf::int32 value) {
  set_has_moudle_type();
  moudle_type_ = value;
}

// optional string user_name = 3;
inline bool MsgClientLogout::has_user_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void MsgClientLogout::set_has_user_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void MsgClientLogout::clear_has_user_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void MsgClientLogout::clear_user_name() {
  if (user_name_ != &::google::protobuf::internal::kEmptyString) {
    user_name_->clear();
  }
  clear_has_user_name();
}
inline const ::std::string& MsgClientLogout::user_name() const {
  return *user_name_;
}
inline void MsgClientLogout::set_user_name(const ::std::string& value) {
  set_has_user_name();
  if (user_name_ == &::google::protobuf::internal::kEmptyString) {
    user_name_ = new ::std::string;
  }
  user_name_->assign(value);
}
inline void MsgClientLogout::set_user_name(const char* value) {
  set_has_user_name();
  if (user_name_ == &::google::protobuf::internal::kEmptyString) {
    user_name_ = new ::std::string;
  }
  user_name_->assign(value);
}
inline void MsgClientLogout::set_user_name(const char* value, size_t size) {
  set_has_user_name();
  if (user_name_ == &::google::protobuf::internal::kEmptyString) {
    user_name_ = new ::std::string;
  }
  user_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MsgClientLogout::mutable_user_name() {
  set_has_user_name();
  if (user_name_ == &::google::protobuf::internal::kEmptyString) {
    user_name_ = new ::std::string;
  }
  return user_name_;
}
inline ::std::string* MsgClientLogout::release_user_name() {
  clear_has_user_name();
  if (user_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = user_name_;
    user_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string ip = 4;
inline bool MsgClientLogout::has_ip() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void MsgClientLogout::set_has_ip() {
  _has_bits_[0] |= 0x00000008u;
}
inline void MsgClientLogout::clear_has_ip() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void MsgClientLogout::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& MsgClientLogout::ip() const {
  return *ip_;
}
inline void MsgClientLogout::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void MsgClientLogout::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void MsgClientLogout::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MsgClientLogout::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* MsgClientLogout::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string mac = 5;
inline bool MsgClientLogout::has_mac() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void MsgClientLogout::set_has_mac() {
  _has_bits_[0] |= 0x00000010u;
}
inline void MsgClientLogout::clear_has_mac() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void MsgClientLogout::clear_mac() {
  if (mac_ != &::google::protobuf::internal::kEmptyString) {
    mac_->clear();
  }
  clear_has_mac();
}
inline const ::std::string& MsgClientLogout::mac() const {
  return *mac_;
}
inline void MsgClientLogout::set_mac(const ::std::string& value) {
  set_has_mac();
  if (mac_ == &::google::protobuf::internal::kEmptyString) {
    mac_ = new ::std::string;
  }
  mac_->assign(value);
}
inline void MsgClientLogout::set_mac(const char* value) {
  set_has_mac();
  if (mac_ == &::google::protobuf::internal::kEmptyString) {
    mac_ = new ::std::string;
  }
  mac_->assign(value);
}
inline void MsgClientLogout::set_mac(const char* value, size_t size) {
  set_has_mac();
  if (mac_ == &::google::protobuf::internal::kEmptyString) {
    mac_ = new ::std::string;
  }
  mac_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MsgClientLogout::mutable_mac() {
  set_has_mac();
  if (mac_ == &::google::protobuf::internal::kEmptyString) {
    mac_ = new ::std::string;
  }
  return mac_;
}
inline ::std::string* MsgClientLogout::release_mac() {
  clear_has_mac();
  if (mac_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = mac_;
    mac_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string client_version = 6;
inline bool MsgClientLogout::has_client_version() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void MsgClientLogout::set_has_client_version() {
  _has_bits_[0] |= 0x00000020u;
}
inline void MsgClientLogout::clear_has_client_version() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void MsgClientLogout::clear_client_version() {
  if (client_version_ != &::google::protobuf::internal::kEmptyString) {
    client_version_->clear();
  }
  clear_has_client_version();
}
inline const ::std::string& MsgClientLogout::client_version() const {
  return *client_version_;
}
inline void MsgClientLogout::set_client_version(const ::std::string& value) {
  set_has_client_version();
  if (client_version_ == &::google::protobuf::internal::kEmptyString) {
    client_version_ = new ::std::string;
  }
  client_version_->assign(value);
}
inline void MsgClientLogout::set_client_version(const char* value) {
  set_has_client_version();
  if (client_version_ == &::google::protobuf::internal::kEmptyString) {
    client_version_ = new ::std::string;
  }
  client_version_->assign(value);
}
inline void MsgClientLogout::set_client_version(const char* value, size_t size) {
  set_has_client_version();
  if (client_version_ == &::google::protobuf::internal::kEmptyString) {
    client_version_ = new ::std::string;
  }
  client_version_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MsgClientLogout::mutable_client_version() {
  set_has_client_version();
  if (client_version_ == &::google::protobuf::internal::kEmptyString) {
    client_version_ = new ::std::string;
  }
  return client_version_;
}
inline ::std::string* MsgClientLogout::release_client_version() {
  clear_has_client_version();
  if (client_version_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = client_version_;
    client_version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string dc_ip = 7;
inline bool MsgClientLogout::has_dc_ip() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void MsgClientLogout::set_has_dc_ip() {
  _has_bits_[0] |= 0x00000040u;
}
inline void MsgClientLogout::clear_has_dc_ip() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void MsgClientLogout::clear_dc_ip() {
  if (dc_ip_ != &::google::protobuf::internal::kEmptyString) {
    dc_ip_->clear();
  }
  clear_has_dc_ip();
}
inline const ::std::string& MsgClientLogout::dc_ip() const {
  return *dc_ip_;
}
inline void MsgClientLogout::set_dc_ip(const ::std::string& value) {
  set_has_dc_ip();
  if (dc_ip_ == &::google::protobuf::internal::kEmptyString) {
    dc_ip_ = new ::std::string;
  }
  dc_ip_->assign(value);
}
inline void MsgClientLogout::set_dc_ip(const char* value) {
  set_has_dc_ip();
  if (dc_ip_ == &::google::protobuf::internal::kEmptyString) {
    dc_ip_ = new ::std::string;
  }
  dc_ip_->assign(value);
}
inline void MsgClientLogout::set_dc_ip(const char* value, size_t size) {
  set_has_dc_ip();
  if (dc_ip_ == &::google::protobuf::internal::kEmptyString) {
    dc_ip_ = new ::std::string;
  }
  dc_ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* MsgClientLogout::mutable_dc_ip() {
  set_has_dc_ip();
  if (dc_ip_ == &::google::protobuf::internal::kEmptyString) {
    dc_ip_ = new ::std::string;
  }
  return dc_ip_;
}
inline ::std::string* MsgClientLogout::release_dc_ip() {
  clear_has_dc_ip();
  if (dc_ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = dc_ip_;
    dc_ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
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

#endif  // PROTOBUF_verify_2fMsgClientLogout_2eproto__INCLUDED
