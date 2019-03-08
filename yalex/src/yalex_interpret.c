
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "yalex_interpret.h"
#include "yalex_parse.h"
#include "yalex_system.h"

char yalex_interpret_token_find_symbol(yalex_world *world, const char *token, const char * require, stack_item **out, unsigned char tokens) {
    if (strcmp(world->stack[world->sp + 1].data.text, token) == 0
        || (token[0] == 'R'
            && strcmp(world->stack[world->sp + 1].data.text, "*") > 0
            && token[2] == world->stack[world->sp + 1].data.text[2])) {
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

        stack_item* out[YALEX_SIZE_MAX_DEPENDABLE_STACK]; //TODO: figure out system max pop req
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
            } else if (resolve != 0) {
                //TODO: handle error
                printf("\nError `%s` resolve: ", yalex_system()->tokens[i].token);
                switch (resolve) {
                    case YALEX_EVAL_ERR_STACK_SHALLOW: printf(" stack too shallow"); break;
                    case YALEX_EVAL_ERR_STACK_WRONG_TYPES: printf(" stack has wrong types"); break;
                    default:
                        break;
                }
                printf("\n");
            }
        }
        if (popped->meta == YALEX_TOKEN_EVAL) {
            yalex_stack_item_clear(popped);
        }
        return 1;
    }
    return 0;
}

char yalex_interpret_lambda_sp(yalex_world *world) {
    if (SP.meta == YALEX_TOKEN_LAMBDA) {
        char found = 0;
        for (int i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
            if (world->lambdas[i].name[0] && strcmp(world->lambdas[i].name, SP.data.text) == 0) {
                //TODO: resolve requirements
                yalex_stack_pop_sp(world);
                if (world->lambdas[i].name[0] == '$') {
                    world->lm--; //anonymous lambdas are one use only
                }
                yalex_parse(world, world->lambdas[i].stack);
                found = 1;
                break;
            }
        }
        if (found == 0) {
            printf("error did not find lambda %s", SP.data.text);
            yalex_stack_pop_sp(world); //pop in order to stop inf loop
        }
        return 1;
    }
    return 0;
}

void yalex_interpret_sp(yalex_world *world) {
    char error = 0;
    error += yalex_interpret_system_sp(world);
    error += yalex_interpret_lambda_sp(world);
    if (error == 0) {
        //TODO: handle error
    }
}