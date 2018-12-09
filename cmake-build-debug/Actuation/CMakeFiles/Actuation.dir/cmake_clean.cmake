file(REMOVE_RECURSE
  "libActuationd_.pdb"
  "libActuationd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Actuation.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
