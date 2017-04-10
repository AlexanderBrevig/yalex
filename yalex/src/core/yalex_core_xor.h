#ifndef YALEX_YALEX_CORE_XOR_H
#define YALEX_YALEX_CORE_XOR_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"
#include "yalex_core_boolean.h"

static error xor_tok(token *tok, stack *stack) {
    error err = {
        .code = NO_ERROR,
        .token = 0
    };
    stack_assert_depth(&err, stack, 2);
    token *b = stack_pop(&err, stack);
    token *a = stack_pop(&err, stack);
    if (token_assert_num(&err, a)!=NO_ERROR){
        err.token = (void*)1;
    }
    if (token_assert_num(&err, b)!=NO_ERROR){
        err.token = (void*)2;
    }
    if (err.code == NO_ERROR){
        uint8_t xor_value = (uint32_t)a->value.number ^ (uint32_t)b->value.number;
        token_deinit(a);
        token_deinit(b);
        token_init(&err, a, "xor", 0);
        token_set_num(a, xor_value);
        stack_push(&err, stack, a);
    }
    return err;
}
#endif
