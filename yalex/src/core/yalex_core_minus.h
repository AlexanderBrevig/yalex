#ifndef YALEX_YALEX_CORE_MINUS_H
#define YALEX_YALEX_CORE_MINUS_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error minus_tok(token *tok, stack *stack)
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
        YDBG(" minus -> ");
        float res = arg1->value.number - arg2->value.number;
        token_deinit(arg1);
        token_deinit(arg2);
        char buf[80];
        yalftoa(res, buf);
        token_init(&err, arg1, buf, 0);
        YDBG(arg1->tok);
        YDBGLN("");
        arg1->isNum = 1;
        arg1->value.number = res;
        stack_push(&err, stack, arg1);
    }
    return err;
}

#endif
