#ifndef YALEX_YALEX_CORE_H
#define YALEX_YALEX_CORE_H

#include "error.h"
#include "lexer.h"

#include "core/yalex_core_print.h"
#include "core/yalex_core_plus.h"

enum {
	BEGIN = 0,
	PRINT,
	PLUS,
	BUILTIN_END
};

void registerCore(error *err, lexer *lex){
	token_init(err, &lex->tokens[PRINT], "print", &print_tok);
	token_init(err, &lex->tokens[PLUS], "+", &plus_tok);
	for (int i = 0; i < BUILTIN_END; i++) lex->tokens[i].isBuiltin = 1;
}

#endif
