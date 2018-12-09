project(FabTranslation)
cmake_minimum_required(VERSION 3.11)

set(FabTranslation_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE FabTranslation_Header ${FabTranslation_Include_Dir}/*.hpp)

add_library(FabTranslation STATIC  ${FabTranslation_Header})
target_include_directories(FabTranslation PRIVATE ${FabTranslation_Include_Dir})

set_target_properties(FabTranslation
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)