add_executable(yalex_tests_app yalex_tests.c)

target_link_libraries(yalex_tests_app m src unity)
 
add_test(yalex_tests yalex_tests_app)