project(DesignOptimization)
cmake_minimum_required(VERSION 3.11)

set(DesignOptimization_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE DesignOptimization_Header ${DesignOptimization_Include_Dir}/*.hpp)

add_library(DesignOptimization STATIC  ${DesignOptimization_Header})
target_include_directories(DesignOptimization PRIVATE ${DesignOptimization_Include_Dir})

set_target_properties(DesignOptimization
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)