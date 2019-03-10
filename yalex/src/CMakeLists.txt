file(GLOB SOURCES ./*.c)

file(GLOB LIB_SRC ./*.c)

add_definitions(/Wall) #VS2017 hackish

list(REMOVE_ITEM LIB_SRC "/demo.c")

add_library(src STATIC ${LIB_SRC})

add_executable(release ${SOURCES})

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(release m)
ENDIF()

target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})