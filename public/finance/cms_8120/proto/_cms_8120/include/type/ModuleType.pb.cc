// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "type/ModuleType.pb.h"

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

const ::google::protobuf::EnumDescriptor* MODULE_TYPE_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_type_2fModuleType_2eproto() {
  protobuf_AddDesc_type_2fModuleType_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "type/ModuleType.proto");
  GOOGLE_CHECK(file != NULL);
  MODULE_TYPE_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_type_2fModuleType_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_type_2fModuleType_2eproto() {
}

void protobuf_AddDesc_type_2fModuleType_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025type/ModuleType.proto\022\010cms_8120*t\n\013MOD"
    "ULE_TYPE\022\032\n\024MT_SERVER_ADJUSTTIME\020\241\234\001\022\032\n\024"
    "MT_SERVER_MEDIAGUARD\020\242\234\001\022\031\n\023MT_SERVER_TR"
    "ANSPORT\020\244\234\001\022\022\n\014MT_CLIENT_CS\020\245\234\001B9\n&com.h"
    "ikvision.finance.fms.cms.hpp.typeB\017Modul"
    "eTypeProto", 210);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "type/ModuleType.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_type_2fModuleType_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_type_2fModuleType_2eproto {
  StaticDescriptorInitializer_type_2fModuleType_2eproto() {
    protobuf_AddDesc_type_2fModuleType_2eproto();
  }
} static_descriptor_initializer_type_2fModuleType_2eproto_;

const ::google::protobuf::EnumDescriptor* MODULE_TYPE_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MODULE_TYPE_descriptor_;
}
bool MODULE_TYPE_IsValid(int value) {
  switch(value) {
    case 20001:
    case 20002:
    case 20004:
    case 20005:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace cms_8120

// @@protoc_insertion_point(global_scope)
