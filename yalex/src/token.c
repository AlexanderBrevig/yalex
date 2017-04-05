#include "token.h"

void token_init(error *err, token *tok, const char *name, token_handler tok_handler) {
    if (tok == 0) { err->code = NULL_ERROR; return; }
	token_deinit(tok);
	tok->action = tok_handler;
	for (int i = 0; i < TOKEN_NAME_MAX_LEN && *name; i++) {
		tok->tok[i] = *name;
		if (*name) name++;
	}
}

void token_deinit(token *tok) {
	if (tok == 0) { return; }
	tok->action = 0;
	for (int i = 0; i < TOKEN_NAME_MAX_LEN; i++) {
		tok->tok[i] = 0;
	}
	tok->value.number = 0;
	tok->length = 0;
	tok->isArray = tok->isStr = tok->isNum = tok->isBuiltin = 0;
}