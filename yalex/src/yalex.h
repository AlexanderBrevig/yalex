#ifndef YALEX_H_
#define YALEX_H_

#include "yalex_util.h"
#include "error.h"
#include "stack.h"
#include "lexer.h"

void yalex_init(error *err, lexer *lex, stack *stk)
{
    error_init(err);
    stack_init(err, stk);
    lexer_init(err, lex);
    YDBGLN("YALEX> initialized");
}

#endif
