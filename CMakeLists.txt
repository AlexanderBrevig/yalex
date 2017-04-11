cmake_minimum_required(VERSION 3.7)
project(yalex C)

# Let's tell it a bit about our environment
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# We declare a variable to select what to build set
SET(TARGET_GROUP production CACHE STRING "What Should I Build?")

# common
  add_subdirectory(yalex/src)

if(TARGET_GROUP STREQUAL release)
# add system specific code here
  # no really, right here

elseif(TARGET_GROUP STREQUAL test)
  include(CTest)

# add deps
  add_subdirectory(yalex/unity) 
  add_subdirectory(yalex/tests) 
else() 
  message(FATAL_ERROR "I don't know the TARGET_GROUP you gave me!")
endif()

