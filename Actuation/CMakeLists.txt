project(Actuation)
cmake_minimum_required(VERSION 3.11)

set(Actuation_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Actuation_Header ${Actuation_Include_Dir}/*.hpp)

add_library(Actuation STATIC  ${Actuation_Header})
target_include_directories(Actuation PRIVATE ${Actuation_Include_Dir})

set_target_properties(Actuation
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)