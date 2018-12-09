file(REMOVE_RECURSE
  "libKinematicsd_.pdb"
  "libKinematicsd_.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/Kinematics.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
