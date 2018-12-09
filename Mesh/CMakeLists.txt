project(Mesh)
cmake_minimum_required(VERSION 3.11)

set(Mesh_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Mesh_Header ${Mesh_Include_Dir}/*.hpp)

add_library(Mesh STATIC  ${Mesh_Header})
target_include_directories(Mesh PRIVATE ${Mesh_Include_Dir})


include(../cmake_utils/cmake_utils.txt)
add_includes(Mesh)

set_target_properties(Mesh
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)
