
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define YALEX_STACK_SIZE 20
#include "yalex.h"
#include "yalex_core.h" //get PRINT token

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
    char buffer[128];
    size_t bufsize = 128;
    while (1) {
        YALEXPRT();
        #if __linux__
            char *ptr = &buffer[0];
            getline(&ptr,&bufsize,stdin);
            lexer_parse(&err, &lex, &stk, buffer);
            /*yalex_run(buffer);/*
            token *tok = stack_peek(&err, &stk);
                    
            if (tok != 0) {
                token_handler handl = (token_handler)lex.tokens[PRINT].action;
                error error = (*handl)(0, &stk);
                YDBGLN("REPLPUSH");
                stack_push(&err, &stk, tok);
            }*/
        #elif _WIN32
            printf("ADD REPL FOR WINDOWS");
            system("pause");
        #else
            #error NO REPL FOR PLATFORM
        #endif
    }
}
