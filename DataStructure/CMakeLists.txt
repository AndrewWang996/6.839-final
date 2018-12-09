project(DataStructure)
cmake_minimum_required(VERSION 3.11)

set(DataStructure_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE DataStructure_Header ${DataStructure_Include_Dir}/*.hpp)

add_library(DataStructure STATIC  ${DataStructure_Header})
target_include_directories(DataStructure PRIVATE ${DataStructure_Include_Dir})

set_target_properties(DataStructure
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)
