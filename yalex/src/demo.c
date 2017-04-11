
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define YALEX_STACK_SIZE 20
#include "yalex.h"

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
    char *programs[1];
    for (int i=0; programs[i] != 0; i++){
        //TODO: add a REPL

        #if __linux__
            char ch = getchar();
        #elif _WIN32
            system("pause");
        #endif
    }
}
