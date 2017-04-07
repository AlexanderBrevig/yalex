#ifndef YALEX_YALEX_CORE_NOT_EQUALS_H
#define YALEX_YALEX_CORE_NOT_EQUALS_H


#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error not_equals_tok(token *tok, stack *stack) {
    error err = {
        .code = NO_ERROR,
        .token = 0
    };
    stack_assert_depth(&err, stack, 2);
    token *arg2 = stack_pop(&err, stack);
    token *arg1 = stack_pop(&err, stack);
    if (err.code == NO_ERROR) {
        YDBG("DEBUG> ");
        YDBG(arg2->tok);
        YDBG(" ");
        YDBG(arg1->tok);
        YDBG(" equals -> ");

        float isEqual = 0;
        uint8_t foundType = 0;
        if (arg1->isStr){
            foundType = 1;
            if (arg2->isStr){
                char *buf1 = (char*)arg1->value.voidptr;
                char *buf2 = (char*)arg2->value.voidptr;
                while (*buf1 == *buf2 && *buf1 != 0 && *buf2 != 0){
                    buf1++;
                    buf2++;
                }
                isEqual = *buf1 == *buf2 ? 1 : 0;
            }else{
                err.code = UNEXPECTED_TYPE;
            }
        }
        if (arg1->isBuiltin){
            foundType = 1;
            if (arg2->isBuiltin){
                isEqual = arg1->tok == arg2->tok ? 1 : 0;
            }else{
                err.code = UNEXPECTED_TYPE;
            }
        }

        if (arg1->isNum){
            foundType = 1;
            if (arg2->isNum){
                float a = arg1->value.number;
                float b = arg2->value.number;
                float delta = a - b;
                isEqual = (delta <  0.001f && delta > -0.001f) ? 1 : 0;
            }else{
                err.code = UNEXPECTED_TYPE;
            }
        }
        if (foundType){
            token_deinit(arg1);
            token_deinit(arg2);
            token *tok = isEqual > 0 ? &token_false : &token_true;
            YDBG(tok->tok);
            YDBGLN("");
            stack_push(&err, stack, tok);
        }else{
            err.code = UNEXPECTED_TYPE;
        }
    }
    return err;
}
#endif