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
    stack_assert_depth(&err, stack, 1);
    token *dupToken = stack_peek(&err, stack);
    if (err.code == NO_ERROR) {
        //lexer *lex = stack->lex;
        /*if (lex == 0) {
            err.code = NO_LEXER;
        } else {*/
            token *newToken = lexer_alloc(&err/*,lex*/);
            if (newToken!=0) {
                token_copy_into(dupToken, newToken);
                stack_push(&err, stack, newToken);
            }else{
                err.code = OUT_OF_MEMORY;
            }
        //}
    }
    return err;
}

#endif
