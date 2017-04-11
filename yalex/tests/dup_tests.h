#ifndef YALEX_DUP_TESTS_H
#define YALEX_DUP_TESTS_H

#include "../unity/unity.h"
#include "yalex_test_helpers.h"

void test_DUP_should_NotReuseExistingToken() {
    PREPARE_DEFAULT_YALEX();
    token original;
    token_init(&err, &original, "orig", 0);
    stack_push(&err, &stk, &original);
    CALL_TOK_ACTON(lex.tokens[DUP], stk);
    TEST_ASSERT_EQUAL_UINT(2, stk.currentIndex);
    TEST_ASSERT_EQUAL(stk.stack[1], stk.stack[1]);
    TEST_ASSERT_NOT_EQUAL(stk.stack[1], stk.stack[2]);
}

#endif