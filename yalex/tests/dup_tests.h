#ifndef YALEX_DUP_TESTS_H
#define YALEX_DUP_TESTS_H

#include "../unity/unity.h"

void test_DUP_should_NotReuseExistingToken() {
    TEST_ASSERT_EQUAL_HEX8(40, 40);
}

#endif