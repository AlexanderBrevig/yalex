#ifndef YALEX_H
#define YALEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "yalex_interop.h"
#include "yalex_config.h"

typedef YALEX_NUMERIC_TYPE numeric_type;
typedef YALEX_ADDRESS_TYPE address_type;

#define YALEX_SIZE_SYS_LAMBDAS_STACK 5

#define YALEX_SIZE_TOKENS 20 + YALEX_SIZE_USER_TOKENS

#define YALEX_TOKEN_UNDEFINED 0
#define YALEX_TOKEN_NAN 1
#define YALEX_TOKEN_NUM 2
#define YALEX_TOKEN_EVAL 3
#define YALEX_TOKEN_LAMBDA 4
#define YALEX_TOKEN_LAMBDA_DEFERRED 5

#define YALEX_EVAL_OK 1
#define YALEX_EVAL_ERR_STACK_SHALLOW -1
#define YALEX_EVAL_ERR_STACK_WRONG_TYPES -2

#define SP world->stack[world->sp]

typedef void(*onResult)(const char* result);

#pragma pack(push, 1)
typedef struct _stack_item {
    char meta;
    union {
        numeric_type number;
        char text[YALEX_SIZE_TOKEN_STR];
    } data;
} stack_item;

typedef struct _lambda {
    char name[YALEX_SIZE_TOKEN_STR];
    char requirements[YALEX_SIZE_MAX_DEPENDABLE_STACK];
    char requirementCount;
    char stack[YALEX_SIZE_LAMBDA_STACK_STR];
} lambda;

typedef struct _yalex_world {
    onResult onResultCallback;
    stack_item stack[YALEX_SIZE_STACK];
    lambda lambdas[YALEX_SIZE_LAMBDAS_STACK];
    numeric_type registers[YALEX_SIZE_REGISTERS];
    YALEX_STACK_POINTER_TYPE sp;
    YALEX_LAMBDA_POINTER_TYPE lm;
    #ifdef YALEX_DEBUG
    unsigned int spMax;
    #endif
} yalex_world;
#pragma pack(pop)

void yalex_print_err(yalex_world *world, const char *str);
void yalex_print_str(yalex_world *world, const char *str);
void yalex_print_token(yalex_world *world, stack_item *item);

stack_item * yalex_stack_pop_sp(yalex_world *world);
void yalex_stack_push_sp(yalex_world *world);
void yalex_stack_item_clear(stack_item *popped);


void yalex_repl(yalex_world *world, const char* repltext);

void yalex_init(yalex_world *world, onResult cb);

#ifdef __cplusplus
}
#endif
#endif
