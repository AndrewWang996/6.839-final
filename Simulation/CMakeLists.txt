project(Simulation)
cmake_minimum_required(VERSION 3.11)

set(Simulation_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Simulation_Header ${Simulation_Include_Dir}/*.hpp)

add_library(Simulation STATIC  ${Simulation_Header})
target_include_directories(Simulation PRIVATE ${Simulation_Include_Dir})

set_target_properties(Simulation
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)