#project(Test)
#
#cmake_minimum_required(VERSION 3.11)
#set(Test_Src_Dir ${PROJECT_SOURCE_DIR}/src)
#
#file(GLOB_RECURSE TEST_SRC ${Test_Src_Dir}/*.cpp)
#
#
#add_executable(Test ${TEST_SRC})
#
#
#
#find_library(GTEST_LIB_DEBUG
#		"gtestd_${LIB_EXTENSION}"
#		PATHS ${PROJECT_SOURCE_DIR}/../ExternalLibs/googletest/googletest/lib/Debug
#		NO_DEFAULT_PATH
#		)
#find_library(GTEST_LIB_RELEASE
#		"gtest_${LIB_EXTENSION}"
#		PATHS ${PROJECT_SOURCE_DIR}}/../ExternalLibs/googletest/googletest/lib/Release
#		NO_DEFAULT_PATH
#		)
#find_library(GTEST_MAIN_LIB_DEBUG
#		"gtest_maind_${LIB_EXTENSION}"
#		PATHS ${PROJECT_SOURCE_DIR}}/../ExternalLibs/googletest/googletest/lib/Debug
#		NO_DEFAULT_PATH
#		)
#find_library(GTEST_MAIN_LIB_RELEASE
#		"gtest_main_${LIB_EXTENSION}"
#		PATHS ${PROJECT_SOURCE_DIR}}/../ExternalLibs/googletest/googletest/lib/Release
#		NO_DEFAULT_
#		)
#
#
#target_include_directories(Test PRIVATE ${PROJECT_SOURCE_DIR}/../ExternalLibs/googletest/googletest/include)
#include(../cmake_utils/cmake_utils.txt)
#add_includes(Test)
#
#target_include_directories(Test PRIVATE ${PROJECT_SOURCE_DIR}/../AutodiffEngine/include)
#
#
#target_link_libraries(Test
#	AutodiffEngine
#	${GTEST_LIBRARIES}
#	${GTEST_MAIN_LIBRARIES}
#	pthread
#)
#
#target_link_libraries(Test
#		debug ${GTEST_LIB_DEBUG}
#		debug ${GTEST_MAIN_LIB_DEBUG}
#		)
#
#target_link_libraries(Test
#		optimized ${GTEST_LIB_RELEASE}
#		optimized ${GTEST_MAIN_LIB_RELEASE}
#		)
#

cmake_minimum_required(VERSION 2.6)
project(Test)

add_executable( Test src/test_autodiff_engine.cpp )

################################
# GTest
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(../cmake_utils/cmake_utils.txt)
add_includes(Test)


include_directories(${PROJECT_SOURCE_DIR}/../AutodiffEngine/include)
include_directories(${PROJECT_SOURCE_DIR}/../Mesh/include)

################################
# Unit Tests
################################

# Link test executable against gtest & gtest_main
target_link_libraries(Test gtest gtest_main)
add_test( Test Test )
