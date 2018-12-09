file(REMOVE_RECURSE
  "libDesignOptimizationd_.pdb"
  "libDesignOptimizationd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/DesignOptimization.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
