// Generated by the protocol buffer compiler.  DO NOT EDIT!
#include "stdafx.h"
#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "cmd.pb.h"

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

const ::google::protobuf::EnumDescriptor* CMD_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_cmd_2eproto() {
  protobuf_AddDesc_cmd_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "cmd.proto");
  GOOGLE_CHECK(file != NULL);
  CMD_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_cmd_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_cmd_2eproto() {
}

void protobuf_AddDesc_cmd_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\tcmd.proto\022\010cms_8120*U\n\003CMD\022&\n!CMD_SERV"
    "ICE_WATCH_DOG_CONTROL_REQ\020\275i\022&\n!CMD_SERV"
    "ICE_WATCH_DOG_CONTROL_RSP\020\276i", 108);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "cmd.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_cmd_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_cmd_2eproto {
  StaticDescriptorInitializer_cmd_2eproto() {
    protobuf_AddDesc_cmd_2eproto();
  }
} static_descriptor_initializer_cmd_2eproto_;

const ::google::protobuf::EnumDescriptor* CMD_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CMD_descriptor_;
}
bool CMD_IsValid(int value) {
  switch(value) {
    case 13501:
    case 13502:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace cms_8120

// @@protoc_insertion_point(global_scope)
