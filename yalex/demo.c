
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
        "10 5 ^ print",
        "0 0 ^ print",
        "1 0 ^ print",
        "0 1 ^ print",
        "1 1 ^ print",
        "1 a ^ print",
        "0 0 ! print",
        "1 0 ! print",
        "0 1 ! print",
        "1 1 ! print",
        "3 6 ! print",
        "1 a ! print",
        "0 0 & print",
        "1 0 & print",
        "0 1 & print",
        "1 1 & print",
        "3 6 & print",
        "1 a & print",
        "3 4 | print",
        "0 0 | print",
        "1 0 | print",
        "0 1 | print",
        "1 1 | print",
        "1 a | print",
        "2 ? ? print",
        "2 ? print",
        "1 2 3 rot clr print",
        "1 2 3 rot print",
        "1 2 pop print",
        "2 dup + print",
        "2 2 = print",
        "2 1 = print",
        "1 2 < print",
        "2 1 < print",
        "2 1 > print",
        "1 2 > print",
        "2 3 * print",
        "3 2 * print",
        "2 0 * print",
        "1 2 / print",
        "2 1 / print",
        "2 0 / print",
        "1 2 - print",
        "2 1 - print",
        "b 1 + print",
        "1 a + print",
        "1 2 + print",
        "-1 2 + print",
        "test print",
        0
    };
    for (int i=0; programs[i] != 0; i++){
        yalex_run(programs[i]);
        printf("\r\n_____\r\n");
        system("pause");
    }
}
