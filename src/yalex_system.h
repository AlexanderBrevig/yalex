#ifndef YALEX_SYSTEM_H
#define YALEX_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yalex.h"

typedef void(*tokenExec)(yalex_world *world, stack_item **out);

#pragma pack(push, 1)
typedef struct _token_built_in {
    #ifndef YALEX_DO_NOT_RESERVE_MEMORY
    const char* token;
    char requirements[YALEX_SIZE_MAX_DEPENDABLE_STACK];
    #else
    char* token;
    char *requirements;
    #endif
    char requirementCount;
    tokenExec exec;
} token_built_in;

typedef struct _yalex_sys {
    #ifndef YALEX_DO_NOT_RESERVE_MEMORY
    token_built_in tokens[YALEX_SIZE_TOKENS];
    lambda lambdas[YALEX_SIZE_SYS_LAMBDAS_STACK];
    #else
    token_built_in *tokens;
    lambda *lambdas;
    #endif
    unsigned char tokenCount;
} yalex_sys;
#pragma pack(pop)

yalex_sys *yalex_system(void);
void yalex_system_init(void);
char yalex_system_token_register(const char* token, char *requirements, char requirementCount, tokenExec exec);

#ifdef __cplusplus
}
#endif
#endif