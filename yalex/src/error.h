#ifndef YALEX_ERROR_H
#define YALEX_ERROR_H

typedef enum _errorcode {
    NO_ERROR,
    STACK_TOO_SHALLOW,
    STACK_POP_FAIL,
    STACK_PEEK_FAIL,
    STACK_OVERFLOW,
	NOT_A_NUMBER,
	OUT_OF_MEMORY,
    NULL_ERROR
} errorcode;

typedef struct _error {
    errorcode code;
    void *token; //TODO : really?
} error;

void error_init(error *err);

#endif
