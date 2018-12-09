#include(FindCSharp.cmake)
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package( CSharp REQUIRED )
#project(Visualization LANGUAGES CSharp)
cmake_minimum_required(VERSION 3.11)
project(Visualization)
include(cmake/UseCSharp.cmake)
#CSHARP_ADD_EXECUTABLE(MyProgram Program.cs)

#TODO: add executable

#TODO
