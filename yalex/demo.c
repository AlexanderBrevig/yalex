
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define YALEX_STACK_SIZE 20
#include "src/yalex.h"

void yalexPrint(const char * format){
    printf(format);
}

token memoryblock[100]; //memory size
token tokens[BUILTIN_TOKENS];
lexer lex = {.variables = memoryblock, .size = 100, .tokens= tokens};
stack stk;
error err;

void yalex_run(char *program){
    error errlocal;
    error_init(&errlocal);
    lexer_parse(&errlocal, &lex, &stk, program);
}

void main() {
    yalex_init(&err, &lex, &stk);

    char *programs[] = {
        "test print",
        "1 2 + print",
        "-1 2 + print",
        "1 2 - print",
        "2 1 - print",
        "1 2 / print",
        "2 1 / print",
        "2 0 / print",
        "2 3 * print",
        "3 2 * print", //10
        "2 0 * print",
        "1 2 < print",
        "2 1 < print",
        "2 1 > print",
        "1 2 > print",
        "2 2 = print",
        "2 1 = print",
        "2 dup + print",
        "1 2 pop print"
    };
    for (int i=16; i<19; i++){
        yalex_run(programs[i]);
        system("pause");
    }
}
