/**/
#include "yalex.h"
//#include "yalex_interop.h"
#include "yalex_tokens.h"
#include "yalex_parse.h"
#include "yalex_interpret.h"
#include "yalex_system.h"

void yalex_print_err(yalex_world *world, const char *str) {
    if (str) {
        char buf[64];
        buf[0] = 0;
        YALEX_STRCAT(buf, 64, "ERR: ");
        YALEX_STRCAT(buf, 64, str);
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
        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        char token[YALEX_SIZE_TOKEN_STR];
        #else 
        char *token = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
        #endif
        YALEX_NUM_TO_STR(item->data.number, token, 10);
        world->onResultCallback(token);
        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        YALEX_FREE(token);
        #endif
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
    #ifdef YALEX_DEBUG
    if (world->sp > world->spMax) {
        world->spMax = world->sp;
    }
    #endif
    if (world->sp >= YALEX_SIZE_STACK) {
        yalex_print_err(world, "STACK OVERFLOW");
        world->sp = YALEX_SIZE_STACK - 1;
    }
}

void yalex_stack_item_clear(stack_item *popped) {
    #ifndef YALEX_DO_NOT_RESERVE_MEMORY
    popped->data.number = 0;
    #else
    //do nothing and expect system to clean up
    #endif
    popped->meta = YALEX_TOKEN_UNDEFINED;
}

void yalex_repl(yalex_world *world, const char* repltext) {
    yalex_parse(world, repltext);
    yalex_print_token(world, &SP);
}



void yalex_init(yalex_world *world, onResult cb) {
    world->sp = 0;
    world->lm = 0;
    #ifdef YALEX_DEBUG
    world->spMax = 0;
    #endif
    yalex_system()->tokenCount = 0;
    world->onResultCallback = cb;

    for (unsigned int i = 0; i < YALEX_SIZE_STACK; i++) {
        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        world->stack[i].data.number = 0;
        #endif
        world->stack[i].meta = YALEX_TOKEN_UNDEFINED;
    }

    for (unsigned int i = 0; i < YALEX_SIZE_REGISTERS; i++) {
        world->registers[i] = 0;
    }

    for (unsigned int i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
        yalex_lambda_init(&world->lambdas[i]);
    }

    yalex_system_init();
}


