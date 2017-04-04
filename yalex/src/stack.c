#include "stack.h"
#include "token.h"

void stack_init(error *err, stack *stk){
    if (err->code != NO_ERROR) return;
    stk->currentIndex = 0;
}

void stack_can_pop(error *err, stack *stk, int argc) {
    if (err->code != NO_ERROR) return;
    if (stk->currentIndex < argc) err->code = STACK_TOO_SHALLOW;
}

token *stack_pop(error *err, stack *stk) {
    uint16_t idx = stk->currentIndex;
    if (err->code != NO_ERROR) {
        return 0;
    }
    if (idx == 0) {
        err->code = STACK_POP_FAIL;
        return 0;
    }
    stk->currentIndex--;
    return stk->stack[idx];
}

token *stack_peek(error *err, stack *stk) {
    uint16_t idx = stk->currentIndex;
    if (err->code != NO_ERROR) {
        return 0;
    }
    if (idx == 0) {
        err->code = STACK_PEEK_FAIL;
        return 0;
    }
    return stk->stack[idx];
}

void stack_push(error *err, stack *stk, token *tok) {
    //todo check
    if (err->code != NO_ERROR) return;
    if (stk->currentIndex + 1 >= YALEX_STACK_SIZE) err->code = STACK_OVERFLOW;
    stk->currentIndex++;
    stk->stack[stk->currentIndex] = tok;
}
