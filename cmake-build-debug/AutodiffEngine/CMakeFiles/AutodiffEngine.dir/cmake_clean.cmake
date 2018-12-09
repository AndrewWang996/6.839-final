file(REMOVE_RECURSE
  "libAutodiffEngined_.pdb"
  "libAutodiffEngined_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/AutodiffEngine.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
