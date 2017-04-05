#ifndef YALEX_YALEX_CORE_PLUS_H
#define YALEX_YALEX_CORE_PLUS_H

#include "../error.h"
#include "../token.h"
#include "../stack.h"
#include "../yalex_util.h"

static error plus_tok(token *tok, stack *stack) {
	error err = {
			.code = NO_ERROR,
			.token = 0
	};
	stack_can_pop(&err, stack, 2);
	//TODO: check num
	token *arg1 = stack_pop(&err, stack);
	token *arg2 = stack_pop(&err, stack);
	if (err.code == NO_ERROR && arg1->isNum != 1) { err.code = NOT_A_NUMBER; err.token = 1; }
	if (err.code == NO_ERROR && arg1->isNum != 1) { err.code = NOT_A_NUMBER; err.token = 2; }
	if (err.code == NO_ERROR) {
		float res = arg1->value.number + arg2->value.number;
		token_deinit(arg1);
		token_deinit(arg2);
		char buf[80];
		yalftoa(res, buf);
		token_init(&err, arg1, buf, 0);
		arg1->isNum = 1;
		arg1->value.number = res;
		stack_push(&err, stack, arg1);
	}
    return err;
}

#endif
