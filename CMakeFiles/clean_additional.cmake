# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\MockOats_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MockOats_autogen.dir\\ParseCache.txt"
  "MockOats_autogen"
  )
endif()
