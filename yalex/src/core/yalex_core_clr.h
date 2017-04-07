#ifndef YALEX_YALEX_CORE_CLR_H
#define YALEX_YALEX_CORE_CLR_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error clr_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_assert_depth(&err, stack, 1);
    if (err.code == NO_ERROR) {
        while (stack->currentIndex >= 1) {
            stack_pop(&err, stack);
        }
    }

    return err;
}
#endif
