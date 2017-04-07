#ifndef YALEX_STACK_H
#define YALEX_STACK_H

#include <stdint.h>
#include "error.h"

#ifndef YALEX_STACK_SIZE
#define YALEX_STACK_SIZE 256
#endif

typedef struct _token token;

typedef struct _stack {
    token *stack[YALEX_STACK_SIZE];
    uint16_t currentIndex;
} stack;

void stack_init(error *err, stack *stk);
void stack_assert_depth(error *err, stack *stk, int argc);
token *stack_pop(error *err, stack *stk);
token *stack_peek(error *err, stack *stk);
void stack_push(error *err, stack *stk, token *tok);
#endif
