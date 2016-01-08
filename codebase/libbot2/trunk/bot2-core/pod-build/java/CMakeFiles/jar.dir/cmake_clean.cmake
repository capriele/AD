FILE(REMOVE_RECURSE
  "CMakeFiles/jar"
  "lcmspy_plugins_bot2.jar"
  "java-build/bot2_spy/ImagePlugin.class"
  "java-build/bot2_spy/PlanarLidarPlugin.class"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/jar.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
