#ifndef YALEX_YALEX_CORE_H
#define YALEX_YALEX_CORE_H

#include "error.h"
#include "lexer.h"

#include "core/yalex_core_print.h"


#include "core/yalex_core_plus.h"
#include "core/yalex_core_minus.h"
#include "core/yalex_core_divide.h"
#include "core/yalex_core_multiply.h"
#include "core/yalex_core_lessthan.h"
#include "core/yalex_core_greaterthan.h"
#include "core/yalex_core_equals.h"

#include "core/yalex_core_dup.h"
#include "core/yalex_core_pop.h"

enum {
    /*YALEX*/
	BEGIN = 0,
    END,
    PRINT,

    /*MATH*/
	PLUS,
    MINUS,
    DIVIDE,
    MULTIPLY,
    LESSTHAN,
    GREATERTHAN,
    EQUALS,
    //SIN,
    //COS,
    //SQRT,
    //POW,

    /*STACK OPERATIONS*/
    DUP,
    POP,

	BUILTIN_END
};
void registerConsts(error *err, lexer *lex);
void registerMath(error *err, lexer *lex);
void registerStackops(error *err, lexer *lex);
void registerYalex(error *err, lexer *lex);

void registerCore(error *err, lexer *lex){
    registerConsts(err, lex);
    registerMath(err, lex);
    registerStackops(err, lex);
    registerYalex(err, lex);
	for (int i = 0; i < BUILTIN_END; i++) lex->tokens[i].isBuiltin = 1;
}

void registerConsts(error *err, lexer *lex){
    token_init(err, &token_true, "true", 0);
    token_true.isBuiltin = 1; token_true.value.number = 1;

    token_init(err, &token_false, "false", 0);
    token_false.isBuiltin = 1; token_false.value.number = 0;
}

void registerMath(error *err, lexer *lex){
    token_init(err, &lex->tokens[PLUS], "+", &plus_tok);
    token_init(err, &lex->tokens[MINUS], "-", &minus_tok);
    token_init(err, &lex->tokens[DIVIDE], "/", &divide_tok);
    token_init(err, &lex->tokens[MULTIPLY], "*", &multiply_tok);
    token_init(err, &lex->tokens[LESSTHAN], "<", &lessthan_tok);
    token_init(err, &lex->tokens[GREATERTHAN], ">", &greaterthan_tok);
    token_init(err, &lex->tokens[EQUALS], "=", &equals_tok);
}

void registerStackops(error *err, lexer *lex){
    token_init(err, &lex->tokens[DUP], "dup", &dup_tok);
    token_init(err, &lex->tokens[POP], "pop", &pop_tok);
}

void registerYalex(error *err, lexer *lex){
    //token_init(err, &lex->tokens[BEGIN], "(", &begin_tok);
    //token_init(err, &lex->tokens[END], ")", &end_tok);
    token_init(err, &lex->tokens[PRINT], "print", &print_tok);
}

#endif