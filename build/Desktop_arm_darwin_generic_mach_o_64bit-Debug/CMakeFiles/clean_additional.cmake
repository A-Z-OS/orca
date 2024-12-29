# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/orca_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/orca_autogen.dir/ParseCache.txt"
  "orca_autogen"
  )
endif()
