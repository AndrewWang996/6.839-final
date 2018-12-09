project(MultiphysicsCoupling)
cmake_minimum_required(VERSION 3.11)

set(MultiphysicsCoupling_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE MultiphysicsCoupling_Header ${MultiphysicsCoupling_Include_Dir}/*.hpp)

add_library(MultiphysicsCoupling STATIC  ${MultiphysicsCoupling_Header})
target_include_directories(MultiphysicsCoupling PRIVATE ${MultiphysicsCoupling_Include_Dir})

set_target_properties(MultiphysicsCoupling
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)