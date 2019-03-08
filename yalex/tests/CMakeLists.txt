add_executable(yalex_tests_app yalex_tests.c)

target_link_libraries(yalex_tests_app src unity)

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(yalex_tests_app m)
ENDIF()
 
add_test(yalex_tests yalex_tests_app)
