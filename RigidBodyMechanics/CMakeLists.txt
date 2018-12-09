project(RigidBodyMechanics)
cmake_minimum_required(VERSION 3.11)

set(RigidBodyMechanics_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE RigidBodyMechanics_Header ${RigidBodyMechanics_Include_Dir}/*.hpp)

add_library(RigidBodyMechanics STATIC  ${RigidBodyMechanics_Header})
target_include_directories(RigidBodyMechanics PRIVATE ${RigidBodyMechanics_Include_Dir})

set_target_properties(RigidBodyMechanics
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)