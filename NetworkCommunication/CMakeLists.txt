project(NetworkCommunication)
cmake_minimum_required(VERSION 3.11)

set(NetworkCommunication_Include_Dir ${PROJECT_SOURCE_DIR}/include)
set(NetworkCommunication_Source_Dir ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE NetworkCommunication_Header ${NetworkCommunication_Include_Dir}/*.hpp ${NetworkCommunication_Include_Dir}/*.h ${NetworkCommunication_Source_Dir}/*.cc)



add_library(NetworkCommunication STATIC  ${NetworkCommunication_Header})
target_include_directories(NetworkCommunication PRIVATE ${NetworkCommunication_Include_Dir})

include(../cmake_utils/cmake_utils.txt)
add_includes(NetworkCommunication)

set_target_properties(NetworkCommunication
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)
