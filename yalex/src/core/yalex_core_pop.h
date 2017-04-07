#ifndef YALEX_YALEX_CORE_POP_H
#define YALEX_YALEX_CORE_POP_H
#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error pop_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_assert_depth(&err, stack, 1);
    if (err.code == NO_ERROR) {
        stack_pop(&err, stack);
    }
    return err;
}
#endif
