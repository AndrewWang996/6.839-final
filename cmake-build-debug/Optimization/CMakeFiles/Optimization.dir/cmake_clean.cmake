file(REMOVE_RECURSE
  "libOptimizationd_.pdb"
  "libOptimizationd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Optimization.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
