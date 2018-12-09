project(Materials)
cmake_minimum_required(VERSION 3.5)

set(Materials_Include_Dir ${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE Materials_Header ${Materials_Include_Dir}/*.hpp)

add_library(Materials STATIC  ${Materials_Header})
target_include_directories(Materials PRIVATE ${Materials_Include_Dir})


include(../cmake_utils/cmake_utils.txt)
add_includes(Materials)

set_target_properties(Materials
        PROPERTIES
        DEBUG_POSTFIX "d_${LIB_EXTENSION}"
        RELEASE_POSTFIX "_${LIB_EXTENSION}"
        LINKER_LANGUAGE CXX
        )

