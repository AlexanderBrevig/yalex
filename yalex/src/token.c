#include "token.h"

void token_init(error *err, token *tok, const char *name, token_handler tok_handler)
{
    if (tok == 0) { err->code = NULL_POINTER_EXCEPTION; return; }
    token_deinit(tok);
    tok->action = tok_handler;
    for (int i = 0; i < TOKEN_NAME_MAX_LEN && *name; i++) {
        tok->tok[i] = *name;
        if (*name) { name++; }
    }
}

void token_deinit(token *tok)
{
    if (tok == 0) { return; }
    tok->action = 0;
    for (int i = 0; i < TOKEN_NAME_MAX_LEN; i++) {
        tok->tok[i] = 0;
    }
    tok->value.number = 0;
    tok->length = 0;
    tok->isArray = tok->isStr = tok->isNum = tok->isBuiltin = 0;
}

void token_copy_into(token *from, token *to)
{
    to->value = from->value;
    to->isBuiltin = from->isBuiltin;
    to->isStr = from->isStr;
    to->isArray = from->isArray;
    to->isNum = from->isNum;
    to->length = from->length;
    to->action = from->action;
    char *cpy = &from->tok[0];
    uint8_t i = 0;
    while (*cpy) {
        to->tok[i] = *cpy;
        cpy++;
        i++;
    }
    to->tok[i] = '\0';
}

token *token_search(token *tokens, uint16_t size, char *buff)
{
    int i = 0;
    // search builtin tokens
    for (i = 0; i < size; i++) {
        if (tokens[i].tok[0] != 0) {
            uint8_t didFindMatch = 0;
            char *tok = &tokens[i].tok[0];
            char *buf = &buff[0];
            while (*tok == *buf && *tok != 0 && *buf != ' ') {
                tok++;
                buf++;
                didFindMatch++;
            }
            if (*tok == 0 && *buf != 0) {
                continue;
            }
            uint8_t bothAt0 = *tok == 0 && *buf == 0;
            tok--;
            buf--;
            if (bothAt0 && *tok == *buf && didFindMatch > 0) {
                return &tokens[i];
            }
        }
    }
    return 0;
}

uint8_t token_assert_num(error *err, token *tok)
{
    if (tok == 0 || tok->isNum == 0) {
        err->code = NOT_A_NUMBER;
    }
    return err->code;
}

void token_clear_type(token *tok)
{
    tok->isNum = tok->isBuiltin = tok->isStr = tok->isArray = 0;
}

void token_set_num(token *tok, float num)
{
    tok->isNum = 1;
    tok->value.number = num;
}