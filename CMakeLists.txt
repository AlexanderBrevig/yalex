cmake_minimum_required(VERSION 3.7)
project(yalex C)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(TARGET_GROUP production CACHE STRING "What Should I Build?")

add_definitions(-DYALEX_DEBUG) #for testing!
add_subdirectory(yalex/src)

if(TARGET_GROUP STREQUAL release)
  # add release deps here
else() 
  # add_definitions(-DYALEX_DEBUG)
  include(CTest)
  add_subdirectory(yalex/Unity) 
  add_subdirectory(yalex/tests) 
endif()