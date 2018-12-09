file(REMOVE_RECURSE
  "libFabTranslationd_.pdb"
  "libFabTranslationd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/FabTranslation.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
