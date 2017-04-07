#ifndef YALEX_YALEX_CORE_NOT_H
#define YALEX_YALEX_CORE_NOT_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error boolean_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_assert_depth(&err, stack, 1);
    token *num = stack_pop(&err, stack);
    token_assert_num(&err, num);
    if (err.code == NO_ERROR || tok != 0){
        if (tok != 0) {
            num = tok;
        }
        if (num != 0) {
            stack_push(&err, stack, num->value.number == 0 ? &token_false : &token_true);
        }
    }
    return err;
}

#endif
