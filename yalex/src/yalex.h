#ifndef YALEX_H_
#define YALEX_H_

#include "yalex_util.h"
#include "lexer.h"

void yalex_init(error *err, lexer *lex, stack *stk) {
    error_init(err);
    lexer_init(err, lex);
    stack_init(err, stk);
}

#endif
