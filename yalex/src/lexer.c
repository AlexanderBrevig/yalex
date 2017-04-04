#include "lexer.h"
#include "yalex_core.h"

token *lexer_search(lexer *lex, char *buff) {
    // search builtin tokens
    for (int i = 0; i < BUILTIN_TOKENS; i++) {
        if (lex->tokens[i].tok != 0) {
            char *tok = &lex->tokens[i].tok[0];
            char *buf = &buff[0];
            while (*tok == *buf && *tok != 0) {
                tok++;
                buf++;
            }
            if (*tok == 0 && *buf == 0) {
                return &lex->tokens[i];
            }
        }
    }
    //search user tokens
    for (int i = 0; i < lex->size; i++) {
        if (lex->memoryBlock[i].tok != 0) {
            char *tok = &lex->memoryBlock[i].tok[0];
            char *buf = &buff[0];
            while (*tok == *buf && *tok != 0) {
                tok++;
                buf++;
            }
            if (*tok == 0 && *buf == 0) {
                return &lex->memoryBlock[i];
            }
        }
    }
    return 0;
}

void lexer_init(error *err, lexer *lex){
    for (int i = 0; i < BUILTIN_TOKENS; i++) {
        for (int j = 0; j < 64; j++) {
            lex->tokens[i].tok[j] = 0;
        }
    }
    for (int i = 0; i < lex->size; i++) {
        for (int j = 0; j < 64; j++) {
            lex->memoryBlock[i].tok[j] = 0;
        }
    }
    registerCore(err, lex);
}

void lexer_parse(error *err, lexer *lex, stack *stk, char *input) {
    char buffer[128];
    int i = 0;
    while (*input) {
        if (*input != ' ' && *(input + 1) != 0) {
            buffer[i++] = *input;
            buffer[i] = 0;
        }
        else {
            buffer[i++] = *input;
            buffer[i] = 0;
            token *tok = lexer_search(lex, buffer);
            if (tok == 0)
            {
                // find available mem
                for (int j = 0; j < 100; j++) {
                    if (lex->memoryBlock[j].tok[0] == 0 && tok == 0) {
                        tok = &lex->memoryBlock[j];
                    }
                }
                if (tok != 0) { //TODO: typing
                    token_init(err, tok, buffer, 0);
                    tok->isStr = 1;
                    stack_push(err, stk, tok);
                }
            }
            else {
                // add exisisnt
                stack_push(err, stk, tok);
            }
            tok = stack_peek(err, stk);
            if (tok != 0 && tok->action != 0) {
                tok = stack_pop(err, stk);
                token_handler handl = (token_handler)tok->action;
                (*handl)(tok, stk);
            }
            i = 0;
        }
        input++;
    }
}