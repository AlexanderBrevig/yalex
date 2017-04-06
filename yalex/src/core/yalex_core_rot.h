#ifndef YALEX_YALEX_CORE_ROT_H_H
#define YALEX_YALEX_CORE_ROT_H_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error rot_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_can_pop(&err, stack, 1);
    if (err.code == NO_ERROR) {
        uint16_t head = stack->currentIndex;
        uint16_t tail = 1;

        while (head != tail | head > tail) {
            token *hd = stack->stack[head];
            token *tl = stack->stack[tail];
            stack->stack[head] = tl;
            stack->stack[tail] = hd;
            tail++;
            head--;
        }
    }

    return err;
}


#endif
