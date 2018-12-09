file(REMOVE_RECURSE
  "libMeshd_.pdb"
  "libMeshd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Mesh.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
