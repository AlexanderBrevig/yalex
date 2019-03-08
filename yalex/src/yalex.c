/**/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "yalex.h"
#include "yalex_tokens.h"
#include "yalex_parse.h"
#include "yalex_interpret.h"
#include "yalex_system.h"

void yalex_print_err(yalex_world *world, const char *str) {
    if (str) {
        char buf[64];
        buf[0] = 0;
        strcat_s(buf, 64, "ERR: ");
        strcat_s(buf, 64, str);
        world->onResultCallback(buf);
    }
}
void yalex_print_str(yalex_world *world, const char *str) {
    if (str) {
        world->onResultCallback(str);
    }
}
void yalex_print_token(yalex_world *world, stack_item *item) {
    if (item->meta == YALEX_TOKEN_NUM) {
        char token[YALEX_SIZE_TOKEN_STR];
        YALEXNTOA(item->data.number, token, YALEX_SIZE_TOKEN_STR, 10);
        world->onResultCallback(token);
    } else {
        if (item->meta != YALEX_TOKEN_UNDEFINED) {
            world->onResultCallback(item->data.text);
        }
    }
}

stack_item * yalex_stack_pop_sp(yalex_world *world) {
    stack_item * ret = &SP;
    world->sp--;
    return ret;
}

void yalex_stack_push_sp(yalex_world *world) { 
    world->sp++; 
    if (world->sp >= YALEX_SIZE_STACK) {
        yalex_print_err(world, "STACK OVERFLOW");
        world->sp = YALEX_SIZE_STACK - 1;
    }
}

void yalex_stack_item_clear(stack_item *popped) {
    popped->data.number = 0;
    popped->meta = YALEX_TOKEN_UNDEFINED;
}

void yalex_repl(yalex_world *world, const char* repltext) {
    yalex_parse(world, repltext);
    yalex_print_token(world, &SP);
}



void yalex_init(yalex_world *world, onResult cb) {
    world->sp = 0;
    world->lm = 0;
    yalex_system()->tokenCount = 0;
    world->onResultCallback = cb;

    for (size_t i = 0; i < YALEX_SIZE_STACK; i++) {
        world->stack[i].data.number = 0;
        world->stack[i].meta = YALEX_TOKEN_UNDEFINED;
    }

    for (size_t i = 0; i < YALEX_SIZE_REGISTERS; i++) {
        world->registers[i] = 0;
    }

    for (size_t i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
        yalex_lambda_init(&world->lambdas[i]);
    }

    yalex_system_init(world);
}


