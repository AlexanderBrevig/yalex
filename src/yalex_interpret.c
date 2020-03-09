#ifdef __cplusplus
extern "C" {
#endif

#include "yalex_interpret.h"
#include "yalex_parse.h"
#include "yalex_system.h"

char yalex_interpret_token_find_symbol(yalex_world *world, const char *token, const char * require, stack_item **out, unsigned char tokens) {
    stack_item *wasSP = &world->stack[world->sp + 1];
    if (wasSP->meta == YALEX_TOKEN_EVAL 
        &&
        (YALEX_STRCMP(wasSP->data.text, token) == 0
            || 
            (token[0] == 'R' && wasSP->data.text && token[2] == wasSP->data.text[2]))) {
        if (world->sp >= tokens) {
            if (*require != -1) { //check requirements if specified
                for (int i = 0; i < tokens; i++) {
                    if (world->stack[world->sp - i].meta != require[tokens - 1 - i]) {
                        return YALEX_EVAL_ERR_STACK_WRONG_TYPES;
                    }
                }
            }
            //if we get here, requirements are met
            for (int i = 0; i < tokens; i++) {
                out[i] = yalex_stack_pop_sp(world);
            }
            return YALEX_EVAL_OK;
        } else {
            return YALEX_EVAL_ERR_STACK_SHALLOW;
        }
    }
    return 0;
}

char yalex_interpret_system_sp(yalex_world *world) {
    if (SP.meta == YALEX_TOKEN_EVAL) {
        stack_item *popped = yalex_stack_pop_sp(world);

        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        stack_item* out[YALEX_SIZE_MAX_DEPENDABLE_STACK];
        #else
        stack_item** out = (stack_item**) YALEX_MALLOC(sizeof(stack_item*) * YALEX_SIZE_MAX_DEPENDABLE_STACK);
        #endif
        for (int i = 0; i < YALEX_SIZE_MAX_DEPENDABLE_STACK; i++) {
            out[i] = 0;
        }


        for (int i = 0; i < yalex_system()->tokenCount; i++) {
            char resolve = yalex_interpret_token_find_symbol(world, yalex_system()->tokens[i].token, yalex_system()->tokens[i].requirements, out, yalex_system()->tokens[i].requirementCount);
            if (resolve == YALEX_EVAL_OK) {
                yalex_system()->tokens[i].exec(world, out);
                for (int r = 0; r < yalex_system()->tokens[i].requirementCount - 1 /*-1 or else we overwrite ourselves*/; r++) {
                    yalex_stack_item_clear(out[r]);
                }
            } else if (resolve < 0) {
                yalex_print_err(world, "Interpret Error:");
                yalex_print_err(world, yalex_system()->tokens[i].token);
                switch (resolve) {
                    case YALEX_EVAL_ERR_STACK_SHALLOW: yalex_print_err(world, "Stack too shallow"); break;
                    case YALEX_EVAL_ERR_STACK_WRONG_TYPES: yalex_print_err(world, "Stack has wrong types"); break;
                    default: 
                        break;
                }
                for (int r = 0; r < yalex_system()->tokens[i].requirementCount; r++) {
                    yalex_stack_pop_sp(world);
                }
            }
        }
        if (popped->meta == YALEX_TOKEN_EVAL) {
            yalex_stack_item_clear(popped);
        }

        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        YALEX_FREE(out);
        #endif
        return 1;
    }
    return 0;
}

char yalex_interpret_lambda_sp(yalex_world *world) {
    stack_item *sp = &SP;
    if (SP.meta == YALEX_TOKEN_LAMBDA) {
        char found = 0;
        for (int i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
            if (world->lambdas[i].name[0] && YALEX_STRCMP(world->lambdas[i].name, SP.data.text) == 0) {
                yalex_stack_pop_sp(world);
                if (world->lambdas[i].name[0] == '$') {
                    world->lm--; //anonymous lambdas are one use only
                }
                yalex_parse(world, world->lambdas[i].stack);
                found = 1;
                return;
            }
        }
        for (int i = 0; i < YALEX_SIZE_SYS_LAMBDAS_STACK; i++) {
            if (yalex_system()->lambdas[i].name[0] && YALEX_STRCMP(yalex_system()->lambdas[i].name, SP.data.text) == 0) {
                yalex_stack_pop_sp(world);
                yalex_parse(world, yalex_system()->lambdas[i].stack);
                found = 1;
                return;
            }
        }
        if (found == 0) {
            #ifndef YALEX_DO_NOT_RESERVE_MEMORY
            char buf[26 + YALEX_SIZE_TOKEN_STR];
            #else
            char *buf = (char *) YALEX_MALLOC(26 + YALEX_SIZE_TOKEN_STR);
            #endif
            buf[0] = 0;
            YALEX_STRCAT(buf, 26 + YALEX_SIZE_TOKEN_STR, "error did not find lambda ");
            YALEX_STRCAT(buf, 26 + YALEX_SIZE_TOKEN_STR, SP.data.text);
            yalex_print_err(world, buf);
            yalex_stack_pop_sp(world); //pop in order to stop inf loop
            #ifdef YALEX_DO_NOT_RESERVE_MEMORY
            YALEX_FREE(buf);
            #endif
        }
        return 1;
    }
    if (SP.meta == YALEX_TOKEN_LAMBDA_DEFERRED) {
        return 1; //silence error report from deferred lambdas that resolve during eval
    }
    return 0;
}

//#include "yalex_tokens.h"
void yalex_interpret_sp(yalex_world *world) {
    //token_dump_exec(world, 0);
    char error = 0;
    error += yalex_interpret_system_sp(world);
    error += yalex_interpret_lambda_sp(world);
    stack_item *spnow = &SP;
    if (error == 0 && SP.meta == YALEX_TOKEN_NAN) {
        yalex_print_err(world, "Interpret Error");
    }
    //getchar();
}
#ifdef __cplusplus
}
#endif