
if (NOT EXISTS "/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/libigl/external/glfw/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/libigl/external/glfw/install_manifest.txt\"")
endif()

file(READ "/home/aespielberg/ResearchCode/OpenFab/cmake-build-debug/ExternalLibs/libigl/external/glfw/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/home/aespielberg/Downloads/CLion-2018.2.2/clion-2018.2.2/bin/cmake/linux/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/home/aespielberg/Downloads/CLion-2018.2.2/clion-2018.2.2/bin/cmake/linux/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

