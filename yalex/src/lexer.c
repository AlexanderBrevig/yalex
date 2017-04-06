#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"
#include "yalex_core.h"


void lexer_init(error *err, lexer *lex) {
	for (int i = 0; i < BUILTIN_TOKENS; i++) {
		token_deinit(&lex->tokens[i]);
	}
	for (int j = 0; j < lex->size; j++) {
		token_deinit(&lex->variables[j]);
	}
	registerCore(err, lex);
}

void lexer_parse(error *err, lexer *lex, stack *stk, char *program) {
	err->program = program;
	char needle[128];
    uint16_t i = 0;
    uint16_t loc = 0;
    char *input = &program[0];
	while (1) {
		if (*input != ' ' && *input != 0 && i<127) {
			needle[i] = *input;
			needle[i + 1] = 0;
			i++;
            loc++;
		}
		else  {
            /// search build in tokens for our current needle
			token *tok = token_search(lex->tokens, BUILTIN_TOKENS, needle);
            if (tok == 0){
                /// search user variables for our current needle
                tok = token_search(lex->variables, lex->size, needle);
            }
			uint8_t newToken = 0;
			if (tok == 0)
			{
				// find available mem
				for (int j = 0; j < lex->size && tok == 0; j++) {
					if (lex->variables[j].tok[0] == 0) {
						tok = &lex->variables[j];
					}
				}
				if (tok != 0) {
					token_init(err, tok, needle, (void*)0);
					if (isdigit(needle[0]) || needle[0] == '-') {
						tok->isNum = 1;
						tok->value.number = (float)atof(needle);
					}
					else {
						tok->isStr = 1;
					}
					stack_push(err, stk, tok);
					newToken = 1;
				}
				else {
					err->code = OUT_OF_MEMORY;
				}
			}
			else {
				stack_push(err, stk, tok);
				newToken = 1;
			}
			if (newToken == 1) {
				tok = stack_peek(err, stk);
				if (err->code == NO_ERROR && tok != 0 && tok->action != 0) {
					tok = stack_pop(err, stk);
					token_handler handl = (token_handler)tok->action;
					error error = (*handl)(tok, stk);
                    err->code = error.code;
                    err->token = error.token;
				}
			}
            err->loc = loc - 1;
            if (error_print(err) != NO_ERROR) {
                return;
            }
			i = 0;
			if (*input == 0) return;
		}
		input++;
	}
}