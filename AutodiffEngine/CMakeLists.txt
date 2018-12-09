project(AutodiffEngine)
cmake_minimum_required(VERSION 3.11)

set(AutodiffEngine_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE AutodiffEngine_Header ${AutodiffEngine_Include_Dir}/*.hpp)

add_library(AutodiffEngine STATIC  ${AutodiffEngine_Header})
target_include_directories(AutodiffEngine PRIVATE ${AutodiffEngine_Include_Dir})

include(../cmake_utils/cmake_utils.txt)
add_includes(AutodiffEngine)


set_target_properties(AutodiffEngine
  PROPERTIES
  DEBUG_POSTFIX "d_${LIB_EXTENSION}"
  RELEASE_POSTFIX "_${LIB_EXTENSION}"
  LINKER_LANGUAGE CXX
)

