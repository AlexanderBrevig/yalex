#ifndef YALEX_ERROR_H
#define YALEX_ERROR_H

#include <stdint.h>

typedef enum _errorcode {
    NO_ERROR = 0,
    STACK_TOO_SHALLOW,
    STACK_POP_FAIL,
    STACK_PEEK_FAIL,
    STACK_OVERFLOW,
    NOT_A_NUMBER,
    OUT_OF_MEMORY,
    NULL_POINTER_EXCEPTION,
    DIVIDE_BY_ZERO,
    UNEXPECTED_TYPE,
    NO_LEXER,
    NUMBER_OF_ERRORS
} errorcode;

static const char *const errors[NUMBER_OF_ERRORS] = {
    "NO_ERROR",
    "STACK_TOO_SHALLOW",
    "STACK_POP_FAIL",
    "STACK_PEEK_FAIL",
    "STACK_OVERFLOW",
    "NOT_A_NUMBER",
    "OUT_OF_MEMORY",
    "NULL_POINTER_EXCEPTION",
    "DIVIDE_BY_ZERO",
    "UNEXPECTED_TYPE",
    "NO_LEXER",
};

typedef struct _error {
    errorcode code;
    char *program;
    uint16_t loc;
    void *token; //TODO : really?
} error;


void error_init(error *err);
errorcode error_print(error *err);

#endif
