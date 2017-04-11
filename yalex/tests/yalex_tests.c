
#include "../unity/unity.h"
#include "tests.h"

void setUp() {}
void tearDown() {}


void main() {
    UNITY_BEGIN();
    //RUN_TEST(test_default_should_StartAtStackIndexZero);

    //RUN_TEST(test_stack_should_IncrementIndexWhenPush);

    RUN_TEST(test_DUP_should_NotReuseExistingToken);
    UNITY_END();

    return;
}
