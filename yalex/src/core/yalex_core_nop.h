#ifndef YALEX_YALEX_CORE_NOP_H
#define YALEX_YALEX_CORE_NOP_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error nop_tok(token *tok, stack *stack)
{
    error err = {
        .code = NO_ERROR,
        .token = 0
    };
    return err;
}

#endif
