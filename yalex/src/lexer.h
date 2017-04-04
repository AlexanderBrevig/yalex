#ifndef YALEX_LEXER_H
#define YALEX_LEXER_H

#include <inttypes.h>
#include "token.h"
#include "stack.h"

#define BUILTIN_TOKENS 30

typedef struct _lexer {
    token *tokens;
    token *memoryBlock;
    uint16_t size;
} lexer;

token *lexer_search(lexer *lex, char *buff);
void lexer_init(error *err, lexer *lex);
void lexer_parse(error *err, lexer *lex, stack *stk, char *input);

#endif