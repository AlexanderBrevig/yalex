#ifndef YALEX_STACK_TESTS_H
#define YALEX_STACK_TESTS_H

#include "../unity/unity.h"
#include "yalex_test_helpers.h"

void test_stack_should_IncrementIndexWhenPush()
{
    PREPARE_DEFAULT_YALEX();
    token original;
    token_init(&err, &original, "orig", 0);
    stack_push(&err, &stk, &original);
    TEST_ASSERT_EQUAL_UINT(1, stk.currentIndex);
}

#endif