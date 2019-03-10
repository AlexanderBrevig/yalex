file(GLOB SOURCES ./*.c)

file(GLOB LIB_SRC ./*.c)

#add_definitions(/Wall) #VS2017 hackish

#add_definitions(-DYALEX_SIZE_STACK=20)
#add_definitions(-DYALEX_SIZE_LAMBDAS_STACK=5)
#add_definitions(-DYALEX_SIZE_LAMBDA_STACK_STR=32)
#add_definitions(-DYALEX_SIZE_REGISTERS=8)
#add_definitions(-DYALEX_NUMERIC_TYPE=int)
#add_definitions(-DYALEX_ADDRESS_TYPE=int)

list(REMOVE_ITEM LIB_SRC "/demo.c")

add_library(src STATIC ${LIB_SRC})

add_executable(release ${SOURCES})

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(release m)
ENDIF()

target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})