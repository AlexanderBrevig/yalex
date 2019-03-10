#ifndef YALEX_H
#define YALEX_H

#include "yalex_interop.h"

#ifndef YALEX_NUMERIC_TYPE
#define YALEX_NUMERIC_TYPE long long
#endif

#ifndef YALEX_ADDRESS_TYPE
#define YALEX_ADDRESS_TYPE long long
#endif

#ifndef YALEX_STRCPY
#define YALEX_STRCPY yalex_strcpy  
#endif
#ifndef YALEX_STRCAT
#define YALEX_STRCAT yalex_strcat
#endif
#ifndef YALEX_STRLEN
#define YALEX_STRLEN yalex_strlen
#endif
#ifndef YALEX_STRCMP
#define YALEX_STRCMP yalex_strcmp
#endif
#ifndef YALEX_MEMSET
#define YALEX_MEMSET yalex_memset
#endif
#ifndef YALEX_NUM_TO_STR
#define YALEX_NUM_TO_STR yalex_lltoa_s
#endif
#ifndef YALEX_STR_TO_NUM
#define YALEX_STR_TO_NUM yalex_atoll_s
#endif


#ifndef YALEX_SIZE_STACK
#define YALEX_SIZE_STACK 100
#endif
#ifndef YALEX_SIZE_TOKEN_STR
#define YALEX_SIZE_TOKEN_STR 22 // digits of numeric_type max -9,223,372,036,854,775,807 + 1
#endif
#ifndef YALEX_SIZE_MAX_DEPENDABLE_STACK
#define YALEX_SIZE_MAX_DEPENDABLE_STACK 4
#endif
#ifndef YALEX_SIZE_USER_TOKENS
#define YALEX_SIZE_USER_TOKENS 10
#endif
#ifndef YALEX_SIZE_LAMBDA_STACK_STR
#define YALEX_SIZE_LAMBDA_STACK_STR 64
#endif
#ifndef YALEX_SIZE_REPL_STR
#define YALEX_SIZE_REPL_STR 128
#endif

#ifndef YALEX_SIZE_SYS_LAMBDAS_STACK
#define YALEX_SIZE_SYS_LAMBDAS_STACK 5
#endif
#ifndef YALEX_SIZE_LAMBDAS_STACK
#define YALEX_SIZE_LAMBDAS_STACK 20
#endif
#ifndef YALEX_SIZE_REGISTERS
#define YALEX_SIZE_REGISTERS 16
#endif

typedef YALEX_NUMERIC_TYPE numeric_type;
typedef YALEX_ADDRESS_TYPE address_type;

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

typedef struct _microlang_world {
    onResult onResultCallback;
    stack_item stack[YALEX_SIZE_STACK];
    lambda lambdas[YALEX_SIZE_LAMBDAS_STACK];
    numeric_type registers[YALEX_SIZE_REGISTERS];
    unsigned char sp;
    unsigned char lm;
} yalex_world;

void yalex_print_err(yalex_world *world, const char *str);
void yalex_print_str(yalex_world *world, const char *str);
void yalex_print_token(yalex_world *world, stack_item *item);

stack_item * yalex_stack_pop_sp(yalex_world *world);
void yalex_stack_push_sp(yalex_world *world);
void yalex_stack_item_clear(stack_item *popped);


void yalex_repl(yalex_world *world, const char* repltext);

void yalex_init(yalex_world *world, onResult cb);
#endif