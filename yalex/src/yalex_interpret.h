#ifndef YALEX_INTERPRET_H
#define YALEX_INTERPRET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yalex.h"

char yalex_interpret_token_find_symbol(yalex_world *world, const char *token, const char * require, stack_item **out, unsigned char tokens);
char yalex_interpret_system_sp(yalex_world *world);
char yalex_interpret_lambda_sp(yalex_world *world);
void yalex_interpret_sp(yalex_world *world);

#ifdef __cplusplus
}
#endif
#endif