#ifndef YALEX_PARSE_H
#define YALEX_PARSE_H

#include "yalex.h"

typedef struct _parse_state {
    char token[YALEX_SIZE_TOKEN_STR];
    char tokenIsNumber;
    unsigned char tokenIdx;
    lambda *lmnew;
    unsigned char lambdaStackIdx;
    unsigned char lambdaParseName;
} parse_state;


void yalex_parse_state_init(parse_state *state);
char * yalex_parse_string(yalex_world *world, parse_state *state, char *code);
char * yalex_parse_lambda_def_undef(yalex_world *world, parse_state *state, lambda *lm, char *code);
char * yalex_parse_lambda_stack(yalex_world *world, parse_state *state, lambda *lm, char *code);
void yalex_parse_token_push_stack(yalex_world *world, const char* token, char tokenIsNumber);
void yalex_parse(yalex_world *world, const char* repltext);

void yalex_lambda_init(lambda *lm);
void yalex_lambda_copy(lambda *to, lambda *from);


#endif