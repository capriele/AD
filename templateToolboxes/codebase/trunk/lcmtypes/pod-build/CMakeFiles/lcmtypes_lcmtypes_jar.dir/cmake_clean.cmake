FILE(REMOVE_RECURSE
  "../lcmtypes/c"
  "../lcmtypes/cpp"
  "../lcmtypes/java"
  "../lcmtypes/python"
  "CMakeFiles/lcmtypes_lcmtypes_jar"
  "lcmtypes_lcmtypes.jar"
  "../lcmtypes/java/agile/pose_t.class"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/lcmtypes_lcmtypes_jar.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
