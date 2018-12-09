project(Optimization)
cmake_minimum_required(VERSION 3.11)

set(Optimization_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Optimization_Header ${Optimization_Include_Dir}/*.hpp)

add_library(Optimization STATIC  ${Optimization_Header})
target_include_directories(Optimization PRIVATE ${Optimization_Include_Dir})

set_target_properties(Optimization
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)