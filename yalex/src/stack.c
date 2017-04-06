#include "stack.h"
#include "token.h"
#include "yalex_util.h"

#define DBGSTACK() \
    YDBG("STACK: "); \
    for (int i=1; i <= stk->currentIndex; i++){ \
        YDBG(stk->stack[i]->tok); \
        YDBG(" "); \
    } \
    YDBGLN("");

void stack_init(error *err, stack *stk){
    if (err->code != NO_ERROR) return;
    stk->currentIndex = 0;
	for (int i = 0; i < YALEX_STACK_SIZE; i++) { stk->stack[i] = 0; }
}

void stack_can_pop(error *err, stack *stk, int argc) {
    if (err->code != NO_ERROR) {
        return;
    }
    if (stk->currentIndex < argc){
        err->code = STACK_TOO_SHALLOW;
    }
}

token *stack_pop(error *err, stack *stk) {
    if (err->code != NO_ERROR) {
        return 0;
    }
    uint16_t idx = stk->currentIndex;
    if (idx == 0) {
        err->code = STACK_POP_FAIL;
        return 0;
    }
    stk->currentIndex--;
	token *ret = stk->stack[idx];
	stk->stack[idx] = 0;
    YDBG("POP__");
    DBGSTACK();
    return ret;
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
    if (err->code != NO_ERROR) {
        return;
    }
	stk->currentIndex++;
	if ((1 + stk->currentIndex) > YALEX_STACK_SIZE) {
        err->code = STACK_OVERFLOW;
        return;
    }
    stk->stack[stk->currentIndex] = tok;
    YDBG("PUSH_");
    DBGSTACK();
}
