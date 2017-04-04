#ifndef YALEX_TOKEN_H
#define YALEX_TOKEN_H

#include <inttypes.h>
#include "error.h"
#include "stack.h"

typedef struct _token {
    char tok[64];
    union _value {
        float fl;
        int64_t number;
        void * voidptr;
    } value;
    uint32_t length;
    //todo refactor
    uint8_t isNum;
    uint8_t isFloat;
    uint8_t isStr;
    uint8_t isArray;
    void *action;
} token;

typedef error (*token_handler)(token *, stack *);
void token_init(error *err, token *tok, const char *name, token_handler tok_handler);

#endif
