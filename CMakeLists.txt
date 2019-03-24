cmake_minimum_required(VERSION 3.7)
project(yalex C)
set_property(TARGET yalex PROPERTY C_STANDARD 99)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(TARGET_GROUP production CACHE STRING "Build target")

#add_definitions(/Wall) #VS2017 hackish

add_definitions(-DYALEX_DEBUG)

#add_definitions(-DYALEX_SIZE_STACK=10)
#add_definitions(-DYALEX_SIZE_TOKEN_STR=11)
#add_definitions(-DYALEX_SIZE_LAMBDAS_STACK=5)
#add_definitions(-DYALEX_SIZE_LAMBDA_STACK_STR=32)
#add_definitions(-DYALEX_SIZE_REGISTERS=8)
#add_definitions(-DYALEX_SIZE_USER_TOKENS=0)
#add_definitions(-DYALEX_SIZE_MAX_DEPENDABLE_STACK=2)
#add_definitions(-DYALEX_NUMERIC_TYPE=int)
#add_definitions(-DYALEX_ADDRESS_TYPE=int)

add_subdirectory(yalex/src)

if(TARGET_GROUP STREQUAL yalex)
  # add release deps here
else() 
  include(CTest)
  add_subdirectory(yalex/Unity) 
  add_subdirectory(yalex/tests) 
endif()