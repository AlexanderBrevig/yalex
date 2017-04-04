#ifndef YALEX_YALEX_CORE_H
#define YALEX_YALEX_CORE_H

#include "error.h"
#include "lexer.h"

#include "core/yalex_core_print.h"

void registerCore(error *err, lexer *lex){
    token_init(err, &lex->tokens[1], "print", &print_tok);
}

#endif
