#include <ctype.h>
#include "lexer.h"
#include "yalex_core.h"

token *lexer_search(lexer *lex, char *buff) {
	int i = 0;
	// search builtin tokens
	for (i = 0; i < BUILTIN_TOKENS; i++) {
		if (lex->tokens[i].tok[0] != 0) {
			char *tok = &lex->tokens[i].tok[0];
			char *buf = &buff[0];
			while (*tok == *buf && *tok != 0 && *buf != ' ') {
				tok++;
				buf++;
			}
			tok--;
			buf--;
			if (*tok == *buf) {
				return &lex->tokens[i];
			}
		}
	}
	//search user tokens
	for (i = 0; i < lex->size; i++) {
		if (lex->memoryBlock[i].tok != 0) {
			char *tok = &lex->memoryBlock[i].tok[0];
			char *buf = &buff[0];
			while (*tok == *buf && *tok != 0 && *buf != ' ') {
				tok++;
				buf++;
			}
			tok--; 
			buf--;
			if (*tok == *buf) {
				return &lex->memoryBlock[i];
			}
		}
	}
	return 0;
}

void lexer_init(error *err, lexer *lex) {
	for (int i = 0; i < BUILTIN_TOKENS; i++) {
		token_deinit(&lex->tokens[i]);
	}
	for (int j = 0; j < lex->size; j++) {
		token_deinit(&lex->memoryBlock[j]);
	}
	registerCore(err, lex);
}

void lexer_parse(error *err, lexer *lex, stack *stk, char *input) {
	char buffer[128];
	int i = 0;
	while (1) {
		if (*input != ' ' && *input != 0) {
			buffer[i] = *input;
			buffer[i + 1] = 0;
			i++;
		}
		else  {
			token *tok = lexer_search(lex, buffer);
			uint8_t newToken = 0;
			if (tok == 0)
			{
				// find available mem
				for (int j = 0; j < lex->size && tok == 0; j++) {
					if (lex->memoryBlock[j].tok[0] == 0) {
						tok = &lex->memoryBlock[j];
					}
				}
				if (tok != 0) {
					token_init(err, tok, buffer, 0);
					if (isdigit(buffer[0]) || buffer[0] == '-') {
						tok->isNum = 1;
						tok->value.number = (float)atof(buffer);
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
				// add exisisnt
				stack_push(err, stk, tok);
				newToken = 1;
			}
			if (newToken == 1) {
				tok = stack_peek(err, stk);
				if (tok != 0 && tok->action != 0) {
					tok = stack_pop(err, stk);
					token_handler handl = (token_handler)tok->action;
					(*handl)(tok, stk);
				}
				if (err->code != NO_ERROR) {
					printf(" error %d ", err->code);
					yalexPrint("ERROR: ");
					//yalexPrint(err->code);
				}
			}
			i = 0;
			if (*input == 0) return;
		}
		input++;
	}
}