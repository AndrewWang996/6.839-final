project(OpenFab)
cmake_minimum_required(VERSION 3.5)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

##TODO: move to global CMake?
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../ExternalLibs/libigl/shared/cmake)

set(OpenFab_Include_Dir ${PROJECT_SOURCE_DIR}/include)
set(OpenFab_Source_Dir ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE OpenFab_Header ${OpenFab_Include_Dir}/*.hpp)
file(GLOB_RECURSE OpenFab_Header ${OpenFab_Include_Dir}/*.h)
file(GLOB_RECURSE OpenFab_Source ${OpenFab_Source_Dir}/*.cpp)
file(GLOB_RECURSE OpenFab_Viewer ${OpenFab_Include_Dir}/igl/viewer/*.cpp)

set(Assignment2_Sources ${OpenFab_Source_Dir}/main2.cpp)
set(Assignment3_Sources ${OpenFab_Source_Dir}/main3.cpp) # ${OpenFab_Viewer})
set(Assignment4_Sources ${OpenFab_Source_Dir}/main4.cpp)
set(Assignment6_Sources ${OpenFab_Source_Dir}/main6.cpp)

#Main setup
add_executable(Assignment2 ${OpenFab_Header} ${Assignment2_Sources})
add_executable(Assignment3 ${OpenFab_Header} ${Assignment3_Sources})
add_executable(Assignment4 ${OpenFab_Header} ${Assignment4_Sources})
add_executable(Assignment6 ${OpenFab_Header} ${Assignment6_Sources})


# Specific includes
target_include_directories(Assignment2 PRIVATE ../DataStructure/include)
target_include_directories(Assignment2 PRIVATE ../Geometry/include)
target_include_directories(Assignment2 PRIVATE ../AutodiffEngine/include)
target_include_directories(Assignment2 PRIVATE ../Mesh/include)
target_include_directories(Assignment2 PRIVATE ../NetworkCommunication/include)
target_include_directories(Assignment2 PRIVATE ../FabTranslation/include)

target_include_directories(Assignment3 PRIVATE ../Deformable/include)
#target_include_directories(Assignment3 PRIVATE ../ExternalLibs/libigl/external/glad)
#target_include_directories(Assignment3 PRIVATE
#  ../ExternalLibs/libigl/external/glad/include)
target_include_directories(Assignment3 PRIVATE
  ../ExternalLibs/libigl/external/glfw/include)
target_include_directories(Assignment3 PRIVATE ../ExternalLibs/libigl/external/eigen)
target_include_directories(Assignment3 PRIVATE ../ExternalLibs/libigl/external/eigen/Eigen)
target_include_directories(Assignment3 PRIVATE ../ExternalLibs/libigl/include)
target_include_directories(Assignment3 PRIVATE include)
target_include_directories(Assignment3 PRIVATE include/igl)
target_include_directories(Assignment3 PRIVATE include/igl/viewer)


target_include_directories(Assignment4 PRIVATE ../Materials/include)
target_include_directories(Assignment4 PRIVATE ../DataStructure/include)
target_include_directories(Assignment4 PRIVATE ../Geometry/include)
target_include_directories(Assignment4 PRIVATE ../AutodiffEngine/include)
target_include_directories(Assignment4 PRIVATE ../Mesh/include)
target_include_directories(Assignment4 PRIVATE ../NetworkCommunication/include)
target_include_directories(Assignment4 PRIVATE ../FabTranslation/include)
target_include_directories(Assignment4 PRIVATE ../ExternalLibs/eigen-git-mirror)


target_include_directories(Assignment6 PRIVATE ../Materials/include)
target_include_directories(Assignment6 PRIVATE ../DataStructure/include)
target_include_directories(Assignment6 PRIVATE ../Geometry/include)
target_include_directories(Assignment6 PRIVATE ../AutodiffEngine/include)
target_include_directories(Assignment6 PRIVATE ../Mesh/include)
target_include_directories(Assignment6 PRIVATE ../NetworkCommunication/include)
target_include_directories(Assignment6 PRIVATE ../FabTranslation/include)
target_include_directories(Assignment6 PRIVATE ../ExternalLibs/eigen-git-mirror)


target_include_directories(glew SYSTEM PUBLIC ../ExternalLibs/glew/include)
target_compile_definitions(glew PUBLIC -DGLEW_BUILD -DGLEW_NO_GLU)


# Generic includes / linking
foreach(Target IN LISTS Assignment2 Assignment3 Assignment4 Assignment6)
  
  target_include_directories(${Target} PRIVATE ${OpenFab_Include_Dir})
  #Add external libraries
  include(../cmake_utils/cmake_utils.txt)
  add_includes(${Target})

#Add other projects

#target_include_directories(OpenFab PRIVATE ${PROJECT_SOURCE_DIR}/../ExternalLibs/eigen-git-mirror/Eigen)
#target_include_directories(OpenFab PRIVATE ${PROJECT_SOURCE_DIR}/../ExternalLibs/eigen-git-mirror/unsupported)
#target_include_directories(OpenFab PRIVATE ${PROJECT_SOURCE_DIR}/../ExternalLibs/CppAD)
#target_include_directories(OpenFab PRIVATE ${PROJECT_SOURCE_DIR}/../ExternalLibs/CppADCodeGen/include)


  set_target_properties(${Target}
    PROPERTIES
    DEBUG_POSTFIX "d_${LIB_EXTENSION}"
    RELEASE_POSTFIX "_${LIB_EXTENSION}"
    LINKER_LANGUAGE CXX
  )
  set(CMAKE_CXX_STANDARD_LIBRARIES -ldl)
  set(CMAKE_VERBOSE_MAKEFILE ON)

  target_link_libraries(${Target}
        glfw
        glad
        NetworkCommunication
        ${PROTOBUF_LIBRARY})

endforeach()

# Specific linkings
target_link_libraries(Assignment3 igl::core igl::opengl igl::opengl_glfw glew)
#target_link_libraries(Assignment3 ${LIBIGL_LIBRARIES} ${LIBIGL_EXTRA_LIBRARIES})

