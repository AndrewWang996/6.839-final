project(MotionPrimitives)
cmake_minimum_required(VERSION 3.11)

set(MotionPrimitives_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE MotionPrimitives_Header ${MotionPrimitives_Include_Dir}/*.hpp)

add_library(MotionPrimitives STATIC  ${MotionPrimitives_Header})
target_include_directories(MotionPrimitives PRIVATE ${MotionPrimitives_Include_Dir})

set_target_properties(MotionPrimitives
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)