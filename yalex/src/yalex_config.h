#ifndef YALEX_CONFIG_H
#define YALEX_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef YALEX_NUMERIC_TYPE
#define YALEX_NUMERIC_TYPE long long
#endif

#ifndef YALEX_ADDRESS_TYPE
#define YALEX_ADDRESS_TYPE long long
#endif

#ifndef YALEX_STACK_POINTER_TYPE
#define YALEX_STACK_POINTER_TYPE unsigned char
#endif
#ifndef YALEX_LAMBDA_POINTER_TYPE
#define YALEX_LAMBDA_POINTER_TYPE unsigned char
#endif

#ifndef YALEX_MALLOC
#define YALEX_MALLOC malloc  
#endif
#ifndef YALEX_FREE
#define YALEX_FREE free  
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

#ifndef YALEX_DO_NOT_RESERVE_MEMORY
#ifndef YALEX_SIZE_STACK
#define YALEX_SIZE_STACK 100
#endif
#ifndef YALEX_SIZE_TOKEN_STR
#define YALEX_SIZE_TOKEN_STR 22 // digits of numeric_type max -9,223,372,036,854,775,807 + 1
#endif
#ifndef YALEX_SIZE_MAX_DEPENDABLE_STACK
#define YALEX_SIZE_MAX_DEPENDABLE_STACK 4
#endif
#if YALEX_SIZE_MAX_DEPENDABLE_STACK < 2
#error YALEX needs at least 2 for YALEX_SIZE_MAX_DEPENDABLE_STACK
#endif

#ifndef YALEX_SIZE_USER_TOKENS
#define YALEX_SIZE_USER_TOKENS 10
#endif
#ifndef YALEX_SIZE_LAMBDA_STACK_STR
#define YALEX_SIZE_LAMBDA_STACK_STR 64
#endif

#ifndef YALEX_SIZE_LAMBDAS_STACK
#define YALEX_SIZE_LAMBDAS_STACK 20
#endif
#ifndef YALEX_SIZE_REGISTERS
#define YALEX_SIZE_REGISTERS 16
#endif
#else
#include "yalex_globals.h"
#include <stdlib.h>
#endif

#ifdef __cplusplus
}
#endif
#endif