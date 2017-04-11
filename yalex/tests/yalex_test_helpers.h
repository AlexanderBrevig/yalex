#ifndef YALEX_TEST_HELPERS_H
#define YALEX_TEST_HELPERS_H

#include "../src/yalex.h"
#include "../src/yalex_core.h"

#define CALL_TOK_ACTON(tok, stk) \
token_handler handl = (token_handler)tok.action; \
error error = (*handl)(0, &stk)
	                    

#define PREPARE_YALEX(MEM) \
	token memoryblock[MEM]; \
	token tokens[BUILTIN_TOKENS]; \
	lexer lex = {.variables = memoryblock, .size = MEM, .tokens= tokens}; \
	stack stk; \
	error err; \
    yalex_init(&err, &lex, &stk)

#define PREPARE_DEFAULT_YALEX() PREPARE_YALEX(100)

#endif