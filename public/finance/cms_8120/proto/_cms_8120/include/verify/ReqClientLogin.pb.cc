// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "verify/ReqClientLogin.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace cms_8120 {

namespace {

const ::google::protobuf::Descriptor* ReqClientLogin_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ReqClientLogin_reflection_ = NULL;
const ::google::protobuf::Descriptor* ReqUserPwdCheck_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ReqUserPwdCheck_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_verify_2fReqClientLogin_2eproto() {
  protobuf_AddDesc_verify_2fReqClientLogin_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "verify/ReqClientLogin.proto");
  GOOGLE_CHECK(file != NULL);
  ReqClientLogin_descriptor_ = file->message_type(0);
  static const int ReqClientLogin_offsets_[8] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, cmd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, moudle_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, user_name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, user_pwd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, mac_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, module_version_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, dc_ip_),
  };
  ReqClientLogin_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ReqClientLogin_descriptor_,
      ReqClientLogin::default_instance_,
      ReqClientLogin_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqClientLogin, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ReqClientLogin));
  ReqUserPwdCheck_descriptor_ = file->message_type(1);
  static const int ReqUserPwdCheck_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqUserPwdCheck, cmd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqUserPwdCheck, login_name_),
  };
  ReqUserPwdCheck_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ReqUserPwdCheck_descriptor_,
      ReqUserPwdCheck::default_instance_,
      ReqUserPwdCheck_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqUserPwdCheck, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ReqUserPwdCheck, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ReqUserPwdCheck));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_verify_2fReqClientLogin_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ReqClientLogin_descriptor_, &ReqClientLogin::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ReqUserPwdCheck_descriptor_, &ReqUserPwdCheck::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_verify_2fReqClientLogin_2eproto() {
  delete ReqClientLogin::default_instance_;
  delete ReqClientLogin_reflection_;
  delete ReqUserPwdCheck::default_instance_;
  delete ReqUserPwdCheck_reflection_;
}

void protobuf_AddDesc_verify_2fReqClientLogin_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::cms_8120::protobuf_AddDesc_type_2fcmd_2eproto();
  ::cms_8120::protobuf_AddDesc_type_2fModuleType_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\033verify/ReqClientLogin.proto\022\010cms_8120\032"
    "\016type/cmd.proto\032\025type/ModuleType.proto\"\227"
    "\001\n\016ReqClientLogin\022\013\n\003cmd\030\001 \001(\005\022\023\n\013moudle"
    "_type\030\002 \001(\005\022\021\n\tuser_name\030\003 \001(\t\022\020\n\010user_p"
    "wd\030\004 \001(\t\022\n\n\002ip\030\005 \001(\t\022\013\n\003mac\030\006 \001(\t\022\026\n\016mod"
    "ule_version\030\007 \001(\t\022\r\n\005dc_ip\030\010 \001(\t\"2\n\017ReqU"
    "serPwdCheck\022\013\n\003cmd\030\001 \001(\005\022\022\n\nlogin_name\030\002"
    " \001(\t", 284);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "verify/ReqClientLogin.proto", &protobuf_RegisterTypes);
  ReqClientLogin::default_instance_ = new ReqClientLogin();
  ReqUserPwdCheck::default_instance_ = new ReqUserPwdCheck();
  ReqClientLogin::default_instance_->InitAsDefaultInstance();
  ReqUserPwdCheck::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_verify_2fReqClientLogin_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_verify_2fReqClientLogin_2eproto {
  StaticDescriptorInitializer_verify_2fReqClientLogin_2eproto() {
    protobuf_AddDesc_verify_2fReqClientLogin_2eproto();
  }
} static_descriptor_initializer_verify_2fReqClientLogin_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int ReqClientLogin::kCmdFieldNumber;
const int ReqClientLogin::kMoudleTypeFieldNumber;
const int ReqClientLogin::kUserNameFieldNumber;
const int ReqClientLogin::kUserPwdFieldNumber;
const int ReqClientLogin::kIpFieldNumber;
const int ReqClientLogin::kMacFieldNumber;
const int ReqClientLogin::kModuleVersionFieldNumber;
const int ReqClientLogin::kDcIpFieldNumber;
#endif  // !_MSC_VER

ReqClientLogin::ReqClientLogin()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ReqClientLogin::InitAsDefaultInstance() {
}

ReqClientLogin::ReqClientLogin(const ReqClientLogin& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ReqClientLogin::SharedCtor() {
  _cached_size_ = 0;
  cmd_ = 0;
  moudle_type_ = 0;
  user_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  user_pwd_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  mac_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  module_version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  dc_ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ReqClientLogin::~ReqClientLogin() {
  SharedDtor();
}

void ReqClientLogin::SharedDtor() {
  if (user_name_ != &::google::protobuf::internal::kEmptyString) {
    delete user_name_;
  }
  if (user_pwd_ != &::google::protobuf::internal::kEmptyString) {
    delete user_pwd_;
  }
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (mac_ != &::google::protobuf::internal::kEmptyString) {
    delete mac_;
  }
  if (module_version_ != &::google::protobuf::internal::kEmptyString) {
    delete module_version_;
  }
  if (dc_ip_ != &::google::protobuf::internal::kEmptyString) {
    delete dc_ip_;
  }
  if (this != default_instance_) {
  }
}

void ReqClientLogin::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ReqClientLogin::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ReqClientLogin_descriptor_;
}

const ReqClientLogin& ReqClientLogin::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_verify_2fReqClientLogin_2eproto();  return *default_instance_;
}

ReqClientLogin* ReqClientLogin::default_instance_ = NULL;

ReqClientLogin* ReqClientLogin::New() const {
  return new ReqClientLogin;
}

void ReqClientLogin::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    cmd_ = 0;
    moudle_type_ = 0;
    if (has_user_name()) {
      if (user_name_ != &::google::protobuf::internal::kEmptyString) {
        user_name_->clear();
      }
    }
    if (has_user_pwd()) {
      if (user_pwd_ != &::google::protobuf::internal::kEmptyString) {
        user_pwd_->clear();
      }
    }
    if (has_ip()) {
      if (ip_ != &::google::protobuf::internal::kEmptyString) {
        ip_->clear();
      }
    }
    if (has_mac()) {
      if (mac_ != &::google::protobuf::internal::kEmptyString) {
        mac_->clear();
      }
    }
    if (has_module_version()) {
      if (module_version_ != &::google::protobuf::internal::kEmptyString) {
        module_version_->clear();
      }
    }
    if (has_dc_ip()) {
      if (dc_ip_ != &::google::protobuf::internal::kEmptyString) {
        dc_ip_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ReqClientLogin::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 cmd = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_moudle_type;
        break;
      }
      
      // optional int32 moudle_type = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_moudle_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &moudle_type_)));
          set_has_moudle_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_user_name;
        break;
      }
      
      // optional string user_name = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_user_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_user_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->user_name().data(), this->user_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_user_pwd;
        break;
      }
      
      // optional string user_pwd = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_user_pwd:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_user_pwd()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->user_pwd().data(), this->user_pwd().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_ip;
        break;
      }
      
      // optional string ip = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_ip:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ip().data(), this->ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_mac;
        break;
      }
      
      // optional string mac = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_mac:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_mac()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->mac().data(), this->mac().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_module_version;
        break;
      }
      
      // optional string module_version = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_module_version:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_module_version()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->module_version().data(), this->module_version().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_dc_ip;
        break;
      }
      
      // optional string dc_ip = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_dc_ip:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_dc_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->dc_ip().data(), this->dc_ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void ReqClientLogin::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 cmd = 1;
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmd(), output);
  }
  
  // optional int32 moudle_type = 2;
  if (has_moudle_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->moudle_type(), output);
  }
  
  // optional string user_name = 3;
  if (has_user_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_name().data(), this->user_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->user_name(), output);
  }
  
  // optional string user_pwd = 4;
  if (has_user_pwd()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_pwd().data(), this->user_pwd().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->user_pwd(), output);
  }
  
  // optional string ip = 5;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->ip(), output);
  }
  
  // optional string mac = 6;
  if (has_mac()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->mac().data(), this->mac().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->mac(), output);
  }
  
  // optional string module_version = 7;
  if (has_module_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->module_version().data(), this->module_version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      7, this->module_version(), output);
  }
  
  // optional string dc_ip = 8;
  if (has_dc_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->dc_ip().data(), this->dc_ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->dc_ip(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ReqClientLogin::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 cmd = 1;
  if (has_cmd()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->cmd(), target);
  }
  
  // optional int32 moudle_type = 2;
  if (has_moudle_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->moudle_type(), target);
  }
  
  // optional string user_name = 3;
  if (has_user_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_name().data(), this->user_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->user_name(), target);
  }
  
  // optional string user_pwd = 4;
  if (has_user_pwd()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_pwd().data(), this->user_pwd().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->user_pwd(), target);
  }
  
  // optional string ip = 5;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->ip(), target);
  }
  
  // optional string mac = 6;
  if (has_mac()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->mac().data(), this->mac().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->mac(), target);
  }
  
  // optional string module_version = 7;
  if (has_module_version()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->module_version().data(), this->module_version().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->module_version(), target);
  }
  
  // optional string dc_ip = 8;
  if (has_dc_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->dc_ip().data(), this->dc_ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->dc_ip(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ReqClientLogin::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 cmd = 1;
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmd());
    }
    
    // optional int32 moudle_type = 2;
    if (has_moudle_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->moudle_type());
    }
    
    // optional string user_name = 3;
    if (has_user_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->user_name());
    }
    
    // optional string user_pwd = 4;
    if (has_user_pwd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->user_pwd());
    }
    
    // optional string ip = 5;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip());
    }
    
    // optional string mac = 6;
    if (has_mac()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->mac());
    }
    
    // optional string module_version = 7;
    if (has_module_version()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->module_version());
    }
    
    // optional string dc_ip = 8;
    if (has_dc_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->dc_ip());
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

void ReqClientLogin::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ReqClientLogin* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ReqClientLogin*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ReqClientLogin::MergeFrom(const ReqClientLogin& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_moudle_type()) {
      set_moudle_type(from.moudle_type());
    }
    if (from.has_user_name()) {
      set_user_name(from.user_name());
    }
    if (from.has_user_pwd()) {
      set_user_pwd(from.user_pwd());
    }
    if (from.has_ip()) {
      set_ip(from.ip());
    }
    if (from.has_mac()) {
      set_mac(from.mac());
    }
    if (from.has_module_version()) {
      set_module_version(from.module_version());
    }
    if (from.has_dc_ip()) {
      set_dc_ip(from.dc_ip());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ReqClientLogin::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ReqClientLogin::CopyFrom(const ReqClientLogin& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ReqClientLogin::IsInitialized() const {
  
  return true;
}

void ReqClientLogin::Swap(ReqClientLogin* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(moudle_type_, other->moudle_type_);
    std::swap(user_name_, other->user_name_);
    std::swap(user_pwd_, other->user_pwd_);
    std::swap(ip_, other->ip_);
    std::swap(mac_, other->mac_);
    std::swap(module_version_, other->module_version_);
    std::swap(dc_ip_, other->dc_ip_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ReqClientLogin::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ReqClientLogin_descriptor_;
  metadata.reflection = ReqClientLogin_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ReqUserPwdCheck::kCmdFieldNumber;
const int ReqUserPwdCheck::kLoginNameFieldNumber;
#endif  // !_MSC_VER

ReqUserPwdCheck::ReqUserPwdCheck()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ReqUserPwdCheck::InitAsDefaultInstance() {
}

ReqUserPwdCheck::ReqUserPwdCheck(const ReqUserPwdCheck& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ReqUserPwdCheck::SharedCtor() {
  _cached_size_ = 0;
  cmd_ = 0;
  login_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ReqUserPwdCheck::~ReqUserPwdCheck() {
  SharedDtor();
}

void ReqUserPwdCheck::SharedDtor() {
  if (login_name_ != &::google::protobuf::internal::kEmptyString) {
    delete login_name_;
  }
  if (this != default_instance_) {
  }
}

void ReqUserPwdCheck::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ReqUserPwdCheck::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ReqUserPwdCheck_descriptor_;
}

const ReqUserPwdCheck& ReqUserPwdCheck::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_verify_2fReqClientLogin_2eproto();  return *default_instance_;
}

ReqUserPwdCheck* ReqUserPwdCheck::default_instance_ = NULL;

ReqUserPwdCheck* ReqUserPwdCheck::New() const {
  return new ReqUserPwdCheck;
}

void ReqUserPwdCheck::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    cmd_ = 0;
    if (has_login_name()) {
      if (login_name_ != &::google::protobuf::internal::kEmptyString) {
        login_name_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ReqUserPwdCheck::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 cmd = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_login_name;
        break;
      }
      
      // optional string login_name = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_login_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_login_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->login_name().data(), this->login_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void ReqUserPwdCheck::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 cmd = 1;
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->cmd(), output);
  }
  
  // optional string login_name = 2;
  if (has_login_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->login_name().data(), this->login_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->login_name(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ReqUserPwdCheck::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 cmd = 1;
  if (has_cmd()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->cmd(), target);
  }
  
  // optional string login_name = 2;
  if (has_login_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->login_name().data(), this->login_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->login_name(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ReqUserPwdCheck::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 cmd = 1;
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmd());
    }
    
    // optional string login_name = 2;
    if (has_login_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->login_name());
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

void ReqUserPwdCheck::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ReqUserPwdCheck* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ReqUserPwdCheck*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ReqUserPwdCheck::MergeFrom(const ReqUserPwdCheck& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_login_name()) {
      set_login_name(from.login_name());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ReqUserPwdCheck::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ReqUserPwdCheck::CopyFrom(const ReqUserPwdCheck& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ReqUserPwdCheck::IsInitialized() const {
  
  return true;
}

void ReqUserPwdCheck::Swap(ReqUserPwdCheck* other) {
  if (other != this) {
    std::swap(cmd_, other->cmd_);
    std::swap(login_name_, other->login_name_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ReqUserPwdCheck::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ReqUserPwdCheck_descriptor_;
  metadata.reflection = ReqUserPwdCheck_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace cms_8120

// @@protoc_insertion_point(global_scope)
