cmake_minimum_required(VERSION 3.7)
project(yalex)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        yalex/src/yalex.h
        yalex/unity/unity.c
        yalex/unity/unity.h
        yalex/unity/unity_internals.h
        yalex/demo.c yalex/src/token.h yalex/src/stack.h yalex/src/error.h yalex/src/stack.c yalex/src/token.c yalex/src/lexer.h yalex/src/lexer.c yalex/src/error.c yalex/src/yalex_core.h yalex/src/yalex_util.h yalex/src/core/yalex_core_print.h yalex/src/core/yalex_core_dup.h yalex/src/core/yalex_core_pop.h yalex/src/core/yalex_core_minus.h yalex/src/core/yalex_core_divide.h yalex/src/core/yalex_core_equals.h yalex/src/core/yalex_core_greaterthan.h yalex/src/core/yalex_core_multiply.h yalex/src/core/yalex_core_lessthan.h yalex/src/core/yalex_core_rot.h yalex/src/core/yalex_core_clr.h yalex/src/core/yalex_core_boolean.h yalex/src/core/yalex_core_or.h yalex/src/core/yalex_core_and.h yalex/src/core/yalex_core_not_equals.h yalex/src/core/yalex_core_xor.h)

add_executable(yalex ${SOURCE_FILES})