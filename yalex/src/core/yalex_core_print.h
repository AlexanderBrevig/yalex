#ifndef YALEX_YALEX_CORE_PRINT_H
#define YALEX_YALEX_CORE_PRINT_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error print_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = "print"
    };
    stack_can_pop(&err, stack, 1);
    token *arg = stack_pop(&err, stack);

    /*if (token->isNum) {
        printf("%d", token->value.number);
    }
    if (token->isFloat) {
        printf("%.6f", token->value.fl);
    }
    if (token->isStr) {
    }*/
    if (arg != 0) yalexPrint((char*)arg->tok);
    else yalexPrint("PRINT ERR");

    return err;
}

#endif
