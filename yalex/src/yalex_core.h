#ifndef YALEX_YALEX_CORE_H
#define YALEX_YALEX_CORE_H

#include "error.h"
#include "token.h"
#include "lexer.h"

#include "core/yalex_core_print.h"
#include "core/yalex_core_nop.h"

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
    BOOLEAN_TOK,
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

void registerCore(error *err, lexer *lex);

#endif