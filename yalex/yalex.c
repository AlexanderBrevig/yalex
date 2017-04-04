#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define false 0
#define true 1

// This is just a PSEUDOCODE implementation brainstorm for myself
typedef enum _yal_token {
	yal_begin = 0,    // structual begin
	yal_print,
	yal_end, // loads of tokens here...
	STD_TOK
} yal_token;

typedef enum _errorcode {
	NO_ERROR,
	STACK_TOO_SHALLOW,
	STACK_POP_FAIL,
	STACK_PEEK_FAIL,
	NULL_ERROR
} errorcode;

typedef struct _error {
	errorcode code;
	yal_token token;
} error;

typedef struct _token {
	char tok[64];
	union _value {
		float fl;
		int64_t number;
		void * voidptr;
	} value;
	uint32_t length;
	//todo refactor
	uint8_t isNum;
	uint8_t isFloat;
	uint8_t isStr;
	uint8_t isArray;
	void* action;
} token;


token memoryblock[100]; //memory size

typedef struct _stack {
	token *stack[100];
	uint16_t currentIndex;
} stack;


token errorToken;

void stack_can_pop(error *err, stack *stk, int argc) {
	if (err->code != NO_ERROR) return;
	if (stk->currentIndex < argc) err->code = STACK_TOO_SHALLOW;
}

token *stack_pop(error *err, stack *stk) {
	uint16_t idx = stk->currentIndex;
	if (err->code != NO_ERROR) {
		return &errorToken;
	}
	if (idx == 0) {
		err->code = STACK_POP_FAIL;
		return &errorToken;
	}
	stk->currentIndex--;
	return stk->stack[idx];
}

token *stack_peek(error *err, stack *stk) {
	uint16_t idx = stk->currentIndex;
	if (err->code != NO_ERROR) {
		return &errorToken;
	}
	if (idx == 0) {
		err->code = STACK_PEEK_FAIL;
		return &errorToken;
	}
	return stk->stack[idx];
}

void stack_push(error *err, stack *stk, token *tok) {
	//todo check
	if (err->code != NO_ERROR) return;
	stk->currentIndex++;
	stk->stack[stk->currentIndex] = tok;
}

#define DO_OR_FAIL(_OP, _ERR, _ERR_CODE) if ( ((_ERR) == NO_ERROR) && !(_OP)) _ERR = _ERR_CODE;
// we've got one function per token
static error print_tok(token *tok, stack *stack) {
	error err = {
		.code = NO_ERROR,
		.token = yal_print
	};
	stack_can_pop(&err, stack, 1);
	token *arg = stack_pop(&err, stack);

	/*if (token->isNum) {
		printf("%d", token->value.number);
	}
	if (token->isFloat) {
		printf("%.6f", token->value.fl);
	}
	if (token->isStr) {
	}*/
	if (arg != 0) printf((char*)arg->tok);
	else printf("PRINT ERR");

	return err;
}

typedef error(*token_handler)(token *, stack *);

void token_init(error *err, token *tok, const char *name, token_handler tok_handler) {
	if (tok == 0) { err->code = NULL_ERROR; return; }
	tok->action = tok_handler;
	for (int i = 0; i < 64; i++) {
		tok->tok[i] = *name;
		if (*name) name++;
	}
	tok->value.number = 0;
	tok->length = 0;
	tok->isArray = tok->isFloat = tok->isStr = tok->isNum = false;
}

// prototypes
token tokens[100];

token *token_lex(char *buff) {
	for (int i = 0; i < 100; i++) {
		if (tokens[i].tok != 0) {
			char *tok = &tokens[i].tok[0];
			char *buf = &buff[0];
			while (*tok == *buf && *tok != 0) {
				tok++;
				buf++;
			}
			if (*tok == 0 && *buf == 0) {
				// found existing token, need to use?
				return &tokens[i];
			}
		}
	}
	return 0;
}

void parse(error *err, stack *stk, char *input) {
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
			token *tok = token_lex(buffer);
			if (tok == 0)
			{
				// find available mem
				for (int j = 0; j < 100; j++) {
					if (memoryblock[j].tok[0] == 0 && tok == 0) {
						tok = &memoryblock[j];
					}
				}
				if (tok != 0) {
					token_init(err, tok, buffer, 0);
					tok->isStr = true;
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
	i = 0;
}

void main() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 64; j++) {
			memoryblock[i].tok[j] = 0;
			tokens[i].tok[j] = 0;
		}
	}
	char *msg = "12 print";
	error err;
	err.code = NO_ERROR;
	stack stk;
	stk.currentIndex = 0;

	token_init(&err, &tokens[1], "print", &print_tok);

	parse(&err, &stk, msg);

	printf("\r\n\r\n");
	system("pause");

}