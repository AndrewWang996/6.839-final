project(HardwareInterface)
cmake_minimum_required(VERSION 3.11)

set(HardwareInterface_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE HardwareInterface_Header ${HardwareInterface_Include_Dir}/*.hpp)

add_library(HardwareInterface STATIC  ${HardwareInterface_Header})
target_include_directories(HardwareInterface PRIVATE ${HardwareInterface_Include_Dir})

set_target_properties(HardwareInterface
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)