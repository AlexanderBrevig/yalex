#ifndef YALEX_DUP_TESTS_H
#define YALEX_DUP_TESTS_H

#include "../unity/unity.h"
#include "yalex_test_helpers.h"

void test_DUP_should_NotReuseExistingToken() {
    PREPARE_DEFAULT_YALEX();
    TEST_ASSERT_EQUAL_UINT(0, stk.currentIndex);
}

#endif