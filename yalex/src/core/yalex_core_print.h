#ifndef YALEX_YALEX_CORE_PRINT_H
#define YALEX_YALEX_CORE_PRINT_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error print_tok(token *tok, stack *stack) {
    error err = {
            .code = NO_ERROR,
            .token = 0
    };
    stack_can_pop(&err, stack, 1);
    token *arg = stack_pop(&err, stack);

	if (arg != 0) {
		if (arg->isNum) {
			char buf[80];
			yalftoa(arg->value.number, buf);
			yalexPrint(buf);
		}
		else {
			yalexPrint((char*)arg->tok);
		}
	}
    else yalexPrint("PRINT ERR");

    return err;
}

#endif
