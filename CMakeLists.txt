project(OpenFab)
# cmake_minimum_required(VERSION 3.11)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake_utils
  ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/libigl/shared/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_LIBRARIES -ldl)

set( Eigen3_VERSION "3.3.4" )

set(CMAKE_CXX_FLAGS -O3 ${CMAKE_CXX_FLAGS})

# Expose PROJECT_SOURCE_DIR to the source code.
# add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DPROJECT_SOURCE_DIR='\"${PROJECT_SOURCE_DIR}\"')

#Add externals
# ExternalProject_Add(Eigen3 ExternalLibs/eigen3)
#add_subdirectory(ExternalLibs/CppAd)
#ExternalProject_Add( Eigen3
#  URL "http://bitbucket.org/eigen/eigen/get/${Eigen3_VERSION}.tar.gz"
#  UPDATE_COMMAND ""
#  CONFIGURE_COMMAND ""
#  BUILD_COMMAND ""
#  INSTALL_COMMAND
#    ${CMAKE_COMMAND} -E copy_directory
#      ${CMAKE_BINARY_DIR}/Eigen3-prefix/src/Eigen3/Eigen
#      ${INSTALL_DEPENDENCIES_DIR}/include/Eigen3/Eigen &&
#    ${CMAKE_COMMAND} -E copy_directory
#      ${CMAKE_BINARY_DIR}/Eigen3-prefix/src/Eigen3/unsupported
#      ${INSTALL_DEPENDENCIES_DIR}/include/Eigen3/unsupported
#)

#set(EIGEN3_INCLUDE_DIR ${INSTALL_DEPENDENCIES_DIR}/include/Eigen3 )

#Add subprojects
#add_subdirectory(ExternalLibs/CppAD)
set(CPPAD_INCLUDE_DIRS "ExternalLibs/CppAD")
#set(GOOGLETEST_SOURCE_DIR "ExternalLibs/googletest/googletest")
add_subdirectory(ExternalLibs/CppADCodeGen)
add_subdirectory(ExternalLibs/googletest)
add_subdirectory(ExternalLibs/glew)
add_subdirectory(ExternalLibs/libigl/external/glfw)
add_subdirectory(ExternalLibs/libigl/external/glad)
#ExternalProject_Add(CppADCodeGen ExternalLibs/CppADCodeGen)

add_subdirectory(Actuation)
add_subdirectory(AutodiffEngine)
add_subdirectory(Control)
add_subdirectory(Deformable)
add_subdirectory(DesignOptimization)
add_subdirectory(FabTranslation)
add_subdirectory(DataStructure)
add_subdirectory(Geometry)
add_subdirectory(HardwareInterface)
add_subdirectory(Kinematics)
add_subdirectory(MachineLearning)
add_subdirectory(Materials)
add_subdirectory(Mesh)
add_subdirectory(MotionPlanning)
add_subdirectory(MotionPrimitives)
add_subdirectory(MultiphysicsCoupling)
add_subdirectory(NetworkCommunication)
add_subdirectory(OpenFab)
add_subdirectory(Optimization)
add_subdirectory(RigidBodyMechanics)
add_subdirectory(Sampling)
add_subdirectory(Simulation)
add_subdirectory(Test)
add_subdirectory(cmake_utils)
if(UNIX)
  add_subdirectory(Visualization)
endif(UNIX)
