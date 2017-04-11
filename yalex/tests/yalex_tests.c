#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define YALEX_STACK_SIZE 20
#include "../src/yalex.h"

#include "../unity/unity.h"
#include "tests.h"

void setUp() {}
void tearDown() {}

token memoryblock[100]; //memory size
token tokens[BUILTIN_TOKENS];
lexer lex = {.variables = memoryblock, .size = 100, .tokens= tokens};
stack stk;
error err;


void main() {
    yalex_init(&err, &lex, &stk);

    UNITY_BEGIN();
    RUN_TEST(test_DUP_should_NotReuseExistingToken);
    UNITY_END();

    return;
}
