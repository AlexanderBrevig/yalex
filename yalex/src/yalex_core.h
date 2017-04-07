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
#include "core/yalex_core_not_equals.h"
#include "core/yalex_core_boolean.h"
#include "core/yalex_core_or.h"
#include "core/yalex_core_and.h"
#include "core/yalex_core_xor.h"

#include "core/yalex_core_dup.h"
#include "core/yalex_core_pop.h"
#include "core/yalex_core_rot.h"
#include "core/yalex_core_clr.h"

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
    LESS_THAN,
    GREATER_THAN,
    EQUALS,
    NOT_EQUALS,
    BOOLEAN,
    AND,
    OR,
    XOR,
    //SIN,
    //COS,
    //SQRT,
    //POW,

    /*STACK OPERATIONS*/
    DUP,
    POP,
    ROT,
    CLR,

    TRUE,
    FALSE,
    NOP,

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
    token_set_num(&token_true, 1);
    lex->tokens[TRUE]= token_true;

    token_init(err, &token_false, "false", 0);
    token_set_num(&token_false, 0);
    lex->tokens[FALSE]= token_false;

    token_init(err, &token_nop, "nop", 0);
    token_false.isBuiltin = 1; token_false.value.voidptr = 0;
    lex->tokens[NOP]= token_nop;
}

void registerMath(error *err, lexer *lex){
    token_init(err, &lex->tokens[PLUS], "+", &plus_tok);
    token_init(err, &lex->tokens[MINUS], "-", &minus_tok);
    token_init(err, &lex->tokens[DIVIDE], "/", &divide_tok);
    token_init(err, &lex->tokens[MULTIPLY], "*", &multiply_tok);
    token_init(err, &lex->tokens[LESS_THAN], "<", &lessthan_tok);
    token_init(err, &lex->tokens[GREATER_THAN], ">", &greaterthan_tok);
    token_init(err, &lex->tokens[EQUALS], "=", &equals_tok);
    token_init(err, &lex->tokens[NOT_EQUALS], "!", &not_equals_tok);
    token_init(err, &lex->tokens[BOOLEAN], "?", &boolean_tok);
    token_init(err, &lex->tokens[OR], "|", &or_tok);
    token_init(err, &lex->tokens[AND], "&", &and_tok);
    token_init(err, &lex->tokens[XOR], "^", &xor_tok);
}

void registerStackops(error *err, lexer *lex){
    token_init(err, &lex->tokens[DUP], "dup", &dup_tok);
    token_init(err, &lex->tokens[POP], "pop", &pop_tok);
    token_init(err, &lex->tokens[ROT], "rot", &rot_tok);
    token_init(err, &lex->tokens[CLR], "clr", &clr_tok);
}

void registerYalex(error *err, lexer *lex){
    //token_init(err, &lex->tokens[BEGIN], "(", &begin_tok);
    //token_init(err, &lex->tokens[END], ")", &end_tok);
    token_init(err, &lex->tokens[PRINT], "print", &print_tok);
}

#endif