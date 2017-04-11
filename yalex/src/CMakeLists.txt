file(GLOB SOURCES ./*.c)

file(GLOB LIB_SRC ./*.c)

list(REMOVE_ITEM LIB_SRC "/demo.c")

add_library(src STATIC ${LIB_SRC})

add_executable(release ${SOURCES})
target_link_libraries(release m)

target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})