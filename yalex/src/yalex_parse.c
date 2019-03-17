#ifdef __cplusplus
extern "C" {
#endif

#include "yalex_parse.h"
#include "yalex_tokens.h"
#include "yalex_interpret.h"
#include "yalex_system.h"

void yalex_parse_state_init(parse_state *state) {
    state->lambdaParseName = 0;
    state->tokenIsNumber = 1;
    state->tokenIdx = 0;
    state->token[0] = 0;
    state->lambdaStackIdx = 0;
    YALEX_MEMSET(state->token, 0, YALEX_SIZE_TOKEN_STR);
    state->lmnew = 0;
}

char * yalex_parse_string(parse_state *state, char *code) {
    state->token[state->tokenIdx++] = *code;
    state->token[state->tokenIdx] = 0;
    code++;
    do {
        state->token[state->tokenIdx++] = *code;
        state->token[state->tokenIdx] = 0;
        if (*code == '"') break;
    } while (code++);
    state->tokenIsNumber = 0;
    return code;
}

char * yalex_parse_lambda_def_undef(yalex_world *world, parse_state *state, lambda *lm, char *code) {
    if (state->token[0] == 0) {
        yalex_lambda_init(lm);
        state->lmnew = lm;
        state->lambdaParseName = 1;
    } else {
        for (unsigned char i = 0; i < world->lm; i++) {
            if (YALEX_STRCMP(state->token, world->lambdas[i].name) == 0) {
                for (int j = i + 1; j <= world->lm; j++) {
                    yalex_lambda_copy(&world->lambdas[j - 1], &world->lambdas[j]);
                }
                world->lm--; //remove
            }
        }

        YALEX_MEMSET(state->token, 0, YALEX_SIZE_TOKEN_STR);
    }
    return code;
}

char * yalex_parse_lambda_stack(yalex_world *world, parse_state *state, lambda *lm, char *code) {
    if (*code == '(') {
        //start stack parser
        if (state->lmnew == 0) {
            yalex_lambda_init(lm);
            state->lmnew = lm;
        }
        return code;
    } else if (*code == ')') {
        //close lambda parser and add
        if (world->lm + 1 < YALEX_SIZE_LAMBDAS_STACK
            && state->lmnew) {
            lambda *lmcpy = 0;
            for (int i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
                if (YALEX_STRLEN(state->lmnew->name) > 0 && YALEX_STRCMP(world->lambdas[i].name, state->lmnew->name) == 0) {
                    lmcpy = &world->lambdas[i];
                }
            }
            if (lmcpy == 0) {
                lmcpy = &world->lambdas[world->lm++];
            }
            if (state->lmnew->requirements[0]) { YALEX_STRCPY(lmcpy->requirements, YALEX_SIZE_MAX_DEPENDABLE_STACK, state->lmnew->requirements); }
            if (state->lmnew->stack[0]) { YALEX_STRCPY(lmcpy->stack, YALEX_SIZE_LAMBDA_STACK_STR, state->lmnew->stack); }
            lmcpy->requirementCount = state->lmnew->requirementCount;
            if (state->lmnew->name[0]) {
                YALEX_STRCPY(lmcpy->name, YALEX_SIZE_TOKEN_STR, state->lmnew->name);
            } else {
                state->token[0] = 0;
                YALEX_STRCAT(state->token, YALEX_SIZE_TOKEN_STR, "$");
                
                #ifndef YALEX_DO_NOT_RESERVE_MEMORY
                char buf[YALEX_SIZE_TOKEN_STR-1];
                #else
                char* buf = (char *) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR-1);
                #endif
                
                YALEX_NUM_TO_STR(world->lm - 1, buf, 10);
                YALEX_STRCAT(state->token, YALEX_SIZE_TOKEN_STR-1, buf);
                YALEX_STRCPY(lmcpy->name, YALEX_SIZE_TOKEN_STR-1, state->token);
                yalex_parse_token_push_stack(world, state->token, 0); //push anonymous lambda as token
                
                #ifdef YALEX_DO_NOT_RESERVE_MEMORY
                YALEX_FREE(buf);
                #endif
            }
            yalex_parse_state_init(state);
        } else {
            yalex_print_err(world, "no room for more lambdas");
        }
        return code;
    } else if (state->lmnew) {
        if (state->lambdaParseName && *code != ' ') {
            //find out what is building
            state->lmnew->name[state->lambdaStackIdx++] = *code;
            state->lmnew->name[state->lambdaStackIdx] = 0;
        } else if (state->lambdaParseName && *code == ' ') {
            state->lambdaParseName = 0;
            state->lambdaStackIdx = 0;
        } else {
            //find out what is building
            if (state->lambdaStackIdx + 1 < YALEX_SIZE_LAMBDA_STACK_STR) {
                state->lmnew->stack[state->lambdaStackIdx++] = *code;
                state->lmnew->stack[state->lambdaStackIdx] = 0;
            } else {
                YALEX_STRCPY(state->lmnew->stack, YALEX_SIZE_LAMBDA_STACK_STR, "ERROR_LAMBDA_STACK_TOO_LONG");
            }
        }
        return code;
    }
    return 0;
}

void yalex_parse_token_push_stack(yalex_world *world, const char* token, char tokenIsNumber) {
    if (token[0] == 0) return;
    //push is a special case, we do not want it to be added to the stack
    //it transparently moves the SP up and retains the data that used to be there
    if (YALEX_STRCMP(token, "push") == 0) {
        token_push_exec(world, 0);
    }
    //pop is a special case, we do not want it to be added to the stack
    //it transparently moves the SP down and retains the data that used to be there
    else if (YALEX_STRCMP(token, "pop") == 0) {
        token_pop_exec(world, 0);
    } else if (token[0] == 'R' && ISDIGIT(token[1]) && token[2] == 'S') {
        stack_item dummy;
        dummy.data.number = token[1] - '0';

        stack_item *dummyptr = &dummy;
        stack_item **fakeout = &dummyptr;
        token_regset_exec(world, fakeout);
    } else {
        yalex_stack_push_sp(world);
        if ((tokenIsNumber && token[0] != '-')
            || (tokenIsNumber && token[0] == '-' && YALEX_STRLEN(token) > 1)) {
            #ifdef YALEX_DO_NOT_RESERVE_MEMORY
            if (SP.meta == YALEX_TOKEN_NAN) {
                YALEX_FREE(SP.data.text); 
            }
            #endif
            SP.meta = YALEX_TOKEN_NUM;
            SP.data.number = YALEX_STR_TO_NUM(token, 0, 10);
        } else if (token[0] == '0' && token[1] == 'x') {
            numeric_type number = YALEX_STR_TO_NUM(token, 0, 16);
            #ifdef YALEX_DO_NOT_RESERVE_MEMORY
            if (SP.meta == YALEX_TOKEN_NAN) {
                YALEX_FREE(SP.data.text);
            }
            #endif
            SP.meta = YALEX_TOKEN_NUM;
            SP.data.number = number;
        } else {
            SP.meta = YALEX_TOKEN_NAN;
            for (int i = 0; i < yalex_system()->tokenCount; i++) {
                if (YALEX_STRCMP(token, yalex_system()->tokens[i].token) == 0
                    || (YALEX_STRCMP(yalex_system()->tokens[i].token, "*") > 0 && token[0] == 'R')) {
                    
                    SP.meta = YALEX_TOKEN_EVAL;
                }
            }

            for (int i = 0; i < world->lm; i++) {
                if (YALEX_STRCMP(token, world->lambdas[i].name) == 0) {
                    SP.meta = YALEX_TOKEN_LAMBDA;
                }
                if (token[0] == '\'') {
                    SP.meta = YALEX_TOKEN_LAMBDA_DEFERRED;
                }
            }
            for (int i = 0; i < YALEX_SIZE_SYS_LAMBDAS_STACK; i++) {
                if (YALEX_STRCMP(token, yalex_system()->lambdas[i].name) == 0) {
                    SP.meta = YALEX_TOKEN_LAMBDA;
                }
                if (token[0] == '\'') {
                    SP.meta = YALEX_TOKEN_LAMBDA_DEFERRED;
                }
            }

            if (token[0] == '$') {
                SP.meta = YALEX_TOKEN_LAMBDA;
            }
            #ifdef YALEX_DO_NOT_RESERVE_MEMORY
            //if (!SP.data.text) {
                SP.data.text = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
            //}
            #endif
            YALEX_STRCPY(SP.data.text, YALEX_SIZE_TOKEN_STR, token);
            char *nam = SP.data.text;
            int x = 0;
        }
    }
    while (SP.meta == YALEX_TOKEN_EVAL || SP.meta == YALEX_TOKEN_LAMBDA) {
        yalex_interpret_sp(world);
    }
}

void yalex_parse(yalex_world *world, const char* repltext) {
    char *code = (char*)repltext;
    lambda lm;
    parse_state parseState;

    #ifdef YALEX_DO_NOT_RESERVE_MEMORY
    lm.requirementCount = 0;
    lm.name = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
    lm.name[0] = 0;
    lm.requirements = (char*) YALEX_MALLOC(YALEX_SIZE_MAX_DEPENDABLE_STACK);
    lm.requirements[0] = 0;
    lm.stack = (char*) YALEX_MALLOC(YALEX_SIZE_LAMBDA_STACK_STR);
    lm.stack[0] = 0;
    parseState.token = (char *) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
    parseState.token[0] = 0;
    #endif
    yalex_parse_state_init(&parseState);
    while (*code) {
        if (*code == '\t' || *code == '\r' || *code == '\n') {
            //skip whitespace
        } else {
            if (*code == '"') { // start string parser
                code = yalex_parse_string(&parseState, code);
            } else if (*code == ' ' && parseState.lmnew == 0) {
                char *codeBefore = code;
                yalex_parse_token_push_stack(world, parseState.token, parseState.tokenIsNumber);
                yalex_parse_state_init(&parseState);
                code = codeBefore;
            } else if (*code == ':' && parseState.lmnew == 0) {
                code = yalex_parse_lambda_def_undef(world, &parseState, &lm, code);
            } else {
                if (parseState.tokenIdx + 1 == YALEX_SIZE_TOKEN_STR) {
                    yalex_print_err(world, "Error: token too long");
                } else {
                    char * codeAt = yalex_parse_lambda_stack(world, &parseState, &lm, code);
                    if (codeAt) {
                        code = codeAt;
                    } else {
                        // parse regular token if not currently parsing lambda stack
                        if (parseState.tokenIsNumber) {
                            parseState.tokenIsNumber = (ISDIGIT(*code) || (parseState.tokenIdx == 0 && *code == '-'));
                        }
                        parseState.token[parseState.tokenIdx++] = *code;
                        parseState.token[parseState.tokenIdx] = 0;
                    }
                }
            }
        }
        code++;
    }
    yalex_parse_token_push_stack(world, parseState.token, parseState.tokenIsNumber);

    #ifdef YALEX_DO_NOT_RESERVE_MEMORY
    YALEX_FREE(lm.name);
    YALEX_FREE(lm.requirements);
    YALEX_FREE(lm.stack);
    YALEX_FREE(parseState.token);
    #endif
}

void yalex_lambda_init(lambda *lm) {
    YALEX_MEMSET(lm->name, 0, YALEX_SIZE_TOKEN_STR);
    YALEX_MEMSET(lm->requirements, 0, YALEX_SIZE_MAX_DEPENDABLE_STACK);
    YALEX_MEMSET(lm->stack, 0, YALEX_SIZE_LAMBDA_STACK_STR);
    lm->requirementCount = 0;
}

void yalex_lambda_copy(lambda *to, lambda *from) {
    YALEX_STRCPY(to->name, YALEX_SIZE_TOKEN_STR, from->name);
    YALEX_STRCPY(to->requirements, YALEX_SIZE_MAX_DEPENDABLE_STACK, from->requirements);
    YALEX_STRCPY(to->stack, YALEX_SIZE_LAMBDA_STACK_STR, from->stack);
    to->requirementCount = from->requirementCount;
}
#ifdef __cplusplus
}
#endif