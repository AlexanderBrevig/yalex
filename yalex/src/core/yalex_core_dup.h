#ifndef YALEX_YALEX_CORE_DUP_H
#define YALEX_YALEX_CORE_DUP_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error dup_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_can_pop(&err, stack, 1);
    token *dupToken = stack_peek(&err, stack);
    if (err.code == NO_ERROR) {
        stack_push(&err, stack, dupToken);
    }
    return err;
}

#endif
