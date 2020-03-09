add_executable(yalex_tests_app yalex_tests.c)
set_property(TARGET yalex_tests_app PROPERTY C_STANDARD 99)

target_link_libraries(yalex_tests_app src unity)

IF (WIN32)
  # set stuff for windows
ELSE()
  target_link_libraries(yalex_tests_app m)
ENDIF()
 
add_test(yalex_tests yalex_tests_app)
