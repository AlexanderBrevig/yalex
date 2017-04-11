#ifndef YALEX_YALEX_CORE_GREATERTHAN_H
#define YALEX_YALEX_CORE_GREATERTHAN_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error greaterthan_tok(token *tok, stack *stack)
{
    error err = {
        .code = NO_ERROR,
        .token = 0
    };
    stack_assert_depth(&err, stack, 2);
    token *arg2 = stack_pop(&err, stack);
    token *arg1 = stack_pop(&err, stack);
    if (err.code == NO_ERROR && arg1->isNum != 1) {
        err.code = NOT_A_NUMBER;
        err.token = (void *)1;
        return err;
    }
    if (err.code == NO_ERROR && arg2->isNum != 1) {
        err.code = NOT_A_NUMBER;
        err.token = (void *)2;
        return err;
    }
    if (err.code == NO_ERROR) {
        YDBG("DEBUG> ");
        YDBG(arg1->tok);
        YDBG(" ");
        YDBG(arg2->tok);
        YDBG(" greaterthan -> ");
        uint8_t res = arg1->value.number > arg2->value.number;
        token_deinit(arg1);
        token_deinit(arg2);
        token *tok = res ? &token_true : &token_false;
        YDBG(tok->tok);
        YDBGLN("");
        stack_push(&err, stack, tok);
    }
    return err;
}
#endif
