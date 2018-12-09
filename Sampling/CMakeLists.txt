project(Sampling)
cmake_minimum_required(VERSION 3.11)

set(Sampling_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Sampling_Header ${Sampling_Include_Dir}/*.hpp)

add_library(Sampling STATIC  ${Sampling_Header})
target_include_directories(Sampling PRIVATE ${Sampling_Include_Dir})

set_target_properties(Sampling
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)