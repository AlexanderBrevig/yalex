#ifndef YALEX_TOKEN_H
#define YALEX_TOKEN_H

#include <inttypes.h>
#include "error.h"
#include "stack.h"

#define TOKEN_NAME_MAX_LEN 32

typedef union _token_value {
	float number;
	void * voidptr;
} token_value;

typedef struct _token {
	void *action;
    char tok[TOKEN_NAME_MAX_LEN];
    uint32_t length;
	token_value value;
    //todo refactor
    uint8_t isNum;
    uint8_t isStr;
    uint8_t isArray;
	uint8_t isBuiltin;
} token;

typedef error (*token_handler)(token *, stack *);
void token_init(error *err, token *tok, const char *name, token_handler tok_handler);
void token_deinit(token *tok);

#endif
