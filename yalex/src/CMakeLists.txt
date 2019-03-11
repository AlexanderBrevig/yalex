file(GLOB SOURCES ./*.c)

file(GLOB LIB_SRC ./*.c)

list(REMOVE_ITEM LIB_SRC "/demo.c")

add_library(src STATIC ${LIB_SRC})

add_executable(release ${SOURCES})
#execute_process( ..\..\..\emsdk\emscripten\1.38.28\emcc.bat yalex_interop.c yalex_parse.c yalex_system.c yalex_tokens.c yalex_interpret.c yalex.c demo.c -s WASM=1 -o yalex.html)

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(release m)
ENDIF()

target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})