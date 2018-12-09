project(Control)
cmake_minimum_required(VERSION 3.11)

set(Control_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Control_Header ${Control_Include_Dir}/*.hpp)

add_library(Control STATIC  ${Control_Header})
target_include_directories(Control PRIVATE ${Control_Include_Dir})

set_target_properties(Control
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)