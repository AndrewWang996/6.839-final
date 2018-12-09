project(MotionPlanning)
cmake_minimum_required(VERSION 3.11)

set(MotionPlanning_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE MotionPlanning_Header ${MotionPlanning_Include_Dir}/*.hpp)

add_library(MotionPlanning STATIC  ${MotionPlanning_Header})
target_include_directories(MotionPlanning PRIVATE ${MotionPlanning_Include_Dir})

set_target_properties(MotionPlanning
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)