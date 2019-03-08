#ifndef YALEX_SYSTEM_H
#define YALEX_SYSTEM_H

#include "yalex.h"

typedef void(*tokenExec)(yalex_world *world, stack_item **out);

typedef struct _token_built_in {
    const char* token;
    char requirements[YALEX_SIZE_MAX_DEPENDABLE_STACK];
    char requirementCount;
    tokenExec exec;
} token_built_in;

typedef struct _yalex_sys {
    token_built_in tokens[YALEX_SIZE_TOKENS];
    unsigned char tokenCount;
    lambda lambdas[YALEX_SIZE_SYS_LAMBDAS_STACK];
} yalex_sys;

yalex_sys *yalex_system();
void yalex_system_init(yalex_world *world);
char yalex_system_token_register(yalex_world *world, const char* token, char *requirements, char requirementCount, tokenExec exec);

#endif