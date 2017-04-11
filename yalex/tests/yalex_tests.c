
#include "../unity/unity.h"
#include "tests.h"

void setUp() {}
void tearDown() {}


void main() {
    UNITY_BEGIN();
    RUN_TEST(test_DUP_should_NotReuseExistingToken);
    UNITY_END();

    return;
}
