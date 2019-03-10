#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\Unity\src\unity.h"
#include "..\src\yalex.h"

#define TEST_ASSERT_SP_META_IS(META) TEST_ASSERT_EQUAL_INT8_MESSAGE(world.stack[world.sp].meta, YALEX_TOKEN_##META, "Expected " #META " on stack")

yalex_world world;
#define TESTS_BUFFER_SIZE 128
char buffer[TESTS_BUFFER_SIZE];
char messageCallbacks = 0;
void onMessageCallback(const char* ptr) {
    memset(buffer, 0, TESTS_BUFFER_SIZE);
    //printf("\nMSG: '%s'\n",ptr);
    messageCallbacks++;
    char *p = (char *) ptr;
    char *b = &buffer[0];
    while (*p) {
        *b = *p;
        b++;
        p++;
    }
    b = 0; //zero terminate
}

void setUp(void) {
    messageCallbacks = 0;
    yalex_init(&world, &onMessageCallback);
}
void tearDown(void) {}

#define DEF_PARSE(literal) \
	void test_parse_##literal(void) { \
		const char * testString = #literal; yalex_repl(&world, testString);	TEST_ASSERT_EQUAL_STRING(testString, buffer); \
	} \

DEF_PARSE(1)
DEF_PARSE(2)
DEF_PARSE(10000)
DEF_PARSE(some_token)

void test_parse_negative(void) {
    const char * testString = "-10";
    yalex_repl(&world, testString);
    TEST_ASSERT_EQUAL_STRING(testString, buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_parse_hex(void) {
    yalex_repl(&world, "0xFF");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("255", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_parse_string(void) {
    const char * testString = "\"hello world\"";
    yalex_repl(&world, testString);
    TEST_ASSERT_SP_META_IS(NAN);
    TEST_ASSERT_EQUAL_STRING(testString, buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_parse_echo_last_token_of_two(void) {
    yalex_repl(&world, "1 2");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("2", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_add(void) {
    yalex_repl(&world, "11 22 +");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("33", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_sub(void) {
    yalex_repl(&world, "22 10 -");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("12", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_mul(void) {
    yalex_repl(&world, "22 10 *");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("220", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_div(void) {
    yalex_repl(&world, "100 5 /");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("20", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_gt(void) {
    yalex_repl(&world, "10 5 >");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);

    yalex_repl(&world, "5 10 >");
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}

void test_basic_op_lt(void) {
    yalex_repl(&world, "5 10 <");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
    yalex_repl(&world, "10 5 <");
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}

void test_basic_op_eq(void) {
    yalex_repl(&world, "10 10 ==");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
    yalex_repl(&world, "10 11 ==");
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}

void test_basic_op_neq(void) {
    yalex_repl(&world, "10 5 !=");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
    yalex_repl(&world, "10 10 !=");
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}

void test_basic_op_or(void) {
    yalex_repl(&world, "1 0 ||");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
    yalex_repl(&world, "0 1 ||");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
    yalex_repl(&world, "1 1 ||");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(3, messageCallbacks);
    yalex_repl(&world, "0 0 ||");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(4, messageCallbacks);
}

void test_basic_op_and(void) {
    yalex_repl(&world, "1 0 &&");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
    yalex_repl(&world, "0 1 &&");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
    yalex_repl(&world, "1 1 &&");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(3, messageCallbacks);
    yalex_repl(&world, "0 0 &&");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(4, messageCallbacks);
}

void test_basic_op_print(void) {
    yalex_repl(&world, "100 print");
    TEST_ASSERT_SP_META_IS(UNDEFINED);
    TEST_ASSERT_EQUAL_STRING("100", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_peek(void) {
    numeric_type x = 1234;
    numeric_type *pt = &x;
    char addr[16];
    address_type y = (address_type) &x;
    sprintf_s(addr, 16, "%lld", y);
    char prog[128] = { 0 };
    strcat_s(prog, 128, addr);
    strcat_s(prog, 128, " peek");
    numeric_type *p = (numeric_type*) y;
    TEST_ASSERT_EQUAL_INT8(x, *p);
    yalex_repl(&world, prog); //something like `923514 peek`
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1234", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_pop(void) {
    yalex_repl(&world, "1 2 3");
    TEST_ASSERT_EQUAL_INT8(3, world.sp);
    yalex_repl(&world, "pop");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8(2, world.sp);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}

void test_basic_op_push(void) {
    yalex_repl(&world, "1 2 3");
    yalex_repl(&world, "pop");
    yalex_repl(&world, "push");
    TEST_ASSERT_EQUAL_INT8(3, world.sp);
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8(3, world.stack[world.sp].data.number);
    TEST_ASSERT_EQUAL_INT8(3, messageCallbacks);
}

void test_basic_op_select_true(void) {
    yalex_repl(&world, "10 5 > 1 0 select");
    TEST_ASSERT_EQUAL_INT8(1, world.sp);
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_basic_op_select_false(void) {
    yalex_repl(&world, "5 10 > 1 0 select");
    TEST_ASSERT_EQUAL_INT8(1, world.sp);
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("0", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_dup(void) {
    yalex_repl(&world, "10 dup");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8(2, world.sp);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_nop(void) {
    yalex_repl(&world, "nop");
    TEST_ASSERT_EQUAL_INT8(0, world.sp);
    TEST_ASSERT_EQUAL_INT8(0, messageCallbacks);
}

void test_basic_op_(void) {
    yalex_repl(&world, "1 2 < 1 _ select");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8(1, world.sp);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_basic_op_dump(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "1 2 3 dump");
    TEST_ASSERT_EQUAL_INT8(3, world.sp);
    TEST_ASSERT_EQUAL_INT8(1, world.lm);
    TEST_ASSERT_EQUAL_INT8(25, messageCallbacks);
}

void test_basic_op_clr(void) {
    yalex_repl(&world, "1 R1S pop");
    yalex_repl(&world, ":lm (1)");
    yalex_repl(&world, "1 2 3");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL(1, world.registers[1]);
    TEST_ASSERT_EQUAL(1, world.lm);
    TEST_ASSERT_EQUAL(3, world.sp);
    yalex_repl(&world, "clr");
    TEST_ASSERT_EQUAL(0, world.registers[1]);
    TEST_ASSERT_EQUAL(0, world.lm);
    TEST_ASSERT_EQUAL(0, world.sp);
}

void test_basic_op_run(void) {
    yalex_repl(&world, "\"1 2 3\" run");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL(3, world.sp);
    TEST_ASSERT_EQUAL_STRING("3", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

/// Stack pack
void test_pack_empty_is_assigned_and_removed_if_called(void) {
    yalex_repl(&world, "()");
    TEST_ASSERT_SP_META_IS(UNDEFINED);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.sp, "() should evaluate and clear stack");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.lm, "() should define and remove from lambdas");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, messageCallbacks, "There should only be one callback for defining the lm and evaluating it");
}

void test_pack_anonymous_is_resolved(void) {
    yalex_repl(&world, "(1)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.sp, "() should evaluate and add 1 to stack");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.lm, "() should define and remove from lambdas");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("1", buffer, "Should add 1 to stack and thus print it as the result of the repl");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, messageCallbacks, "There should only be one callback for defining the lm and evaluating it");
}

void test_pack_two_anonymous_is_resolved(void) {
    yalex_repl(&world, "(1) (2)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(2, world.sp, "() should evaluate and add 1 2 to stack");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.lm, "() should define and remove from lambdas");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("2", buffer, "Should add 1 to stack and thus print it as the result of the repl");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, messageCallbacks, "There should only be one callback for defining the lm and evaluating it");
}
void test_pack_two_anonymous_is_resolved_then_add(void) {
    yalex_repl(&world, "(1) (2) +");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.sp, "() should evaluate and add 1 2 + then evaluate + to make 3 on stack");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("3", buffer, "Should add 1 to stack and thus print it as the result of the repl");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, messageCallbacks, "There should only be one callback for defining the lm and evaluating it");
}
void test_pack_as_lambda(void) {
    yalex_repl(&world, ":tok (42)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.sp, "should evaluate and add nothing to stack");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.lm, "should define and add to lambdas");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, messageCallbacks, "There should only be no callback for defining a lambda");
}
void test_pack_lambda_redef(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, ":tok (12)");
    yalex_repl(&world, "tok");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.sp, "should leave 12 on stack after redefine");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.lm, "should define one lambda");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("12", buffer, "should leave 12 on stack and then print");
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_pack_lambda_recall(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "tok");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("42", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_pack_lambda_recall_twice(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "tok");
    yalex_repl(&world, "tok");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("42", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}
void test_pack_lambda_undef(void) {
    yalex_repl(&world, ":tok1 (41)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(1, world.lm, "should define and add to lambdas, tok1");
    yalex_repl(&world, ":tok2 (42)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(2, world.lm, "should define and add to lambdas, tok1 & tok2");
    yalex_repl(&world, ":tok3 (42)");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(3, world.lm, "should define and add to lambdas, tok1 & tok2 & tok3");
    yalex_repl(&world, "tok2:");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(2, world.lm, "should undefine tok2 and remove from lambdas");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("tok3", world.lambdas[world.lm - 1].name, "after remove, current lambda should be tok3");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, world.sp, "should evaluate and add nothing to stack");
}
void test_pack_lambda_deferred_no_exec(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "2 1 < 'tok 1 select");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("1", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_pack_lambda_deferred_exec(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "2 1 > 'tok 1 select");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("42", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_pack_lambda_deferred_resolve(void) {
    yalex_repl(&world, ":tok (42)");
    yalex_repl(&world, "'tok resolve");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_MESSAGE(1, world.sp, "resolve and tok will, well, resolve");
    TEST_ASSERT_EQUAL_STRING("42", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_register_set(void) {
    yalex_repl(&world, "1 R0S");
    TEST_ASSERT_EQUAL_MESSAGE(1, world.sp, "register set are non destructive");
    TEST_ASSERT_EQUAL(1, world.registers[0]);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}
void test_register_set_lambda_alias(void) {
    yalex_repl(&world, ":r0s (R0S)");
    yalex_repl(&world, "1 r0s");
    TEST_ASSERT_EQUAL_MESSAGE(1, world.sp, "register set are non destructive");
    TEST_ASSERT_EQUAL(1, world.registers[0]);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_register_get(void) {
    yalex_repl(&world, "1 R0S 2 R1S");
    yalex_repl(&world, "R0R R1R +");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL(1, world.registers[0]);
    TEST_ASSERT_EQUAL(2, world.registers[1]);
    TEST_ASSERT_EQUAL_MESSAGE(3, world.sp, "register set are non destructive, stack is 1 2 3");
    TEST_ASSERT_EQUAL_STRING("3", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}
void test_register_get_lambda_alias(void) {
    yalex_repl(&world, ":r0r (R0R)");
    yalex_repl(&world, ":r1r (R1R)");
    yalex_repl(&world, "1 R0S 2 R1S");
    yalex_repl(&world, "r0r r1r +");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL(1, world.registers[0]);
    TEST_ASSERT_EQUAL(2, world.registers[1]);
    TEST_ASSERT_EQUAL_MESSAGE(3, world.sp, "register set are non destructive, stack is 1 2 3");
    TEST_ASSERT_EQUAL_STRING("3", buffer);
    TEST_ASSERT_EQUAL_INT8(2, messageCallbacks);
}


void test_fault_double_space(void) {
    yalex_repl(&world, "1  2");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL_STRING("2", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_fault_whitespace(void) {
    yalex_repl(&world, "test\t\r\n");
    TEST_ASSERT_SP_META_IS(NAN);
    TEST_ASSERT_EQUAL_STRING("test", buffer);
    TEST_ASSERT_EQUAL_INT8(1, messageCallbacks);
}

void test_fault_remove_interpreted(void) {
    yalex_repl(&world, "1 2 +");
    TEST_ASSERT_SP_META_IS(NUM);
    TEST_ASSERT_EQUAL(3, world.stack[1].data.number);
    TEST_ASSERT_NOT_EQUAL('+', world.stack[3].data.text[0]);
}

void test_fault_err_clears_owned_items(void) {
    yalex_repl(&world, "1 resolve");
    TEST_ASSERT_EQUAL_UINT8(0, world.sp);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(3, messageCallbacks, "3 -> ERROR token cause");
}

int main() {
    UnityBegin("YALEX_TESTS");
    RUN_TEST(test_parse_negative);

    RUN_TEST(test_parse_1);
    RUN_TEST(test_parse_2);
    RUN_TEST(test_parse_10000);
    RUN_TEST(test_parse_some_token);
    RUN_TEST(test_parse_string);
    RUN_TEST(test_parse_echo_last_token_of_two);
    RUN_TEST(test_parse_hex);

    RUN_TEST(test_basic_op_add);
    RUN_TEST(test_basic_op_sub);
    RUN_TEST(test_basic_op_mul);
    RUN_TEST(test_basic_op_div);

    RUN_TEST(test_basic_op_gt);
    RUN_TEST(test_basic_op_lt);
    RUN_TEST(test_basic_op_eq);
    RUN_TEST(test_basic_op_neq);

    RUN_TEST(test_basic_op_or);
    RUN_TEST(test_basic_op_and);

    RUN_TEST(test_basic_op_peek);

    RUN_TEST(test_basic_op_pop);
    RUN_TEST(test_basic_op_push);
    RUN_TEST(test_basic_op_dup);

    RUN_TEST(test_basic_op_nop);
    RUN_TEST(test_basic_op_);

    RUN_TEST(test_basic_op_select_true);
    RUN_TEST(test_basic_op_select_false);

    RUN_TEST(test_basic_op_print);
    RUN_TEST(test_basic_op_dump);

    RUN_TEST(test_basic_op_clr);

    RUN_TEST(test_basic_op_run);


    RUN_TEST(test_pack_empty_is_assigned_and_removed_if_called);
    RUN_TEST(test_pack_anonymous_is_resolved);
    RUN_TEST(test_pack_two_anonymous_is_resolved);
    RUN_TEST(test_pack_two_anonymous_is_resolved_then_add);
    RUN_TEST(test_pack_as_lambda);
    RUN_TEST(test_pack_lambda_redef);
    RUN_TEST(test_pack_lambda_recall);
    RUN_TEST(test_pack_lambda_recall_twice);
    RUN_TEST(test_pack_lambda_undef);
    RUN_TEST(test_pack_lambda_deferred_no_exec);
    RUN_TEST(test_pack_lambda_deferred_exec);
    RUN_TEST(test_pack_lambda_deferred_resolve);

    RUN_TEST(test_register_set);
    RUN_TEST(test_register_set_lambda_alias);
    RUN_TEST(test_register_get);
    RUN_TEST(test_register_get_lambda_alias);

    RUN_TEST(test_fault_whitespace);
    RUN_TEST(test_fault_double_space);
    RUN_TEST(test_fault_remove_interpreted);
    RUN_TEST(test_fault_err_clears_owned_items);
    int ret = UNITY_END();

    getchar();

    return ret;
}
