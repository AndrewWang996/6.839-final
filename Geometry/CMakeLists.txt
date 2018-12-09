project(Geometry)
cmake_minimum_required(VERSION 3.11)

set(Geometry_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Geometry_Header ${Geometry_Include_Dir}/*.hpp)

add_library(Geometry STATIC  ${Geometry_Header})
target_include_directories(Geometry PRIVATE ${Geometry_Include_Dir})

set_target_properties(Geometry
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)