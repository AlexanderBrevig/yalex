
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
lexer lex = {.memoryBlock = memoryblock, .size = 100, .tokens= tokens};
error err;
stack stk;

void main() {
    yalex_init(&err, &lex, &stk);

    char *msg = "1.2 5.7 + print"; 
    lexer_parse(&err, &lex, &stk, msg);

    yalexPrint("\r\n\r\n");
    system("pause");
}
