file(REMOVE_RECURSE
  "libHardwareInterfaced_.pdb"
  "libHardwareInterfaced_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/HardwareInterface.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
