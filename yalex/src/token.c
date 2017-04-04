#include "token.h"

void token_init(error *err, token *tok, const char *name, token_handler tok_handler) {
    if (tok == 0) { err->code = NULL_ERROR; return; }
    tok->action = tok_handler;
    for (int i = 0; i < 64; i++) {
        tok->tok[i] = *name;
        if (*name) name++;
    }
    tok->value.number = 0;
    tok->length = 0;
    tok->isArray = tok->isFloat = tok->isStr = tok->isNum = 0;
}