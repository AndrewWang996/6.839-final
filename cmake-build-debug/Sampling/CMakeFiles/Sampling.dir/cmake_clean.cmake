file(REMOVE_RECURSE
  "libSamplingd_.pdb"
  "libSamplingd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Sampling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
