project(Kinematics)
cmake_minimum_required(VERSION 3.11)

set(Kinematics_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Kinematics_Header ${Kinematics_Include_Dir}/*.hpp)

add_library(Kinematics STATIC  ${Kinematics_Header})
target_include_directories(Kinematics PRIVATE ${Kinematics_Include_Dir})

set_target_properties(Kinematics
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)