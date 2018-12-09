file(REMOVE_RECURSE
  "libControld_.pdb"
  "libControld_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Control.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
