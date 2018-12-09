# Install script for directory: /home/aespielberg/ResearchCode/OpenFab

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/CppADCodeGen/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/googletest/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/libigl/external/glfw/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/libigl/external/glad/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Actuation/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/AutodiffEngine/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Control/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Deformable/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/DesignOptimization/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/FabTranslation/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/DataStructure/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Geometry/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/HardwareInterface/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Kinematics/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/MachineLearning/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Materials/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Mesh/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/MotionPlanning/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/MotionPrimitives/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/MultiphysicsCoupling/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/NetworkCommunication/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/OpenFab/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Optimization/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/RigidBodyMechanics/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Sampling/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Simulation/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Test/cmake_install.cmake")
  include("/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/Visualization/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
