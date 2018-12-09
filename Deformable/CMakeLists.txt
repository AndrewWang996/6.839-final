project(Deformable)
cmake_minimum_required(VERSION 3.5)

set(Deformable_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Deformable_Header ${Deformable_Include_Dir}/*.hpp)

add_library(Deformable STATIC  ${Deformable_Header})
target_include_directories(Deformable PRIVATE ${Deformable_Include_Dir})

set_target_properties(Deformable
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)
