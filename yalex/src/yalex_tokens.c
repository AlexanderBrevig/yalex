#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "yalex_tokens.h"

void token_add_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number + out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}
void token_min_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number - out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_mul_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number * out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_div_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number / out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_lt_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number < out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_gt_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number > out[0]->data.number;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_eq_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number == out[0]->data.number ? 1 : 0;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_neq_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = out[1]->data.number != out[0]->data.number ? 1 : 0;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_or_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = (out[1]->data.number || out[0]->data.number) ? 1 : 0;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_and_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.data.number = (out[1]->data.number && out[0]->data.number) ? 1 : 0;
    SP.meta = YALEX_TOKEN_NUM;
}

void token_print_exec(yalex_world *world, stack_item **out) {
    yalex_print_token(world, &SP);
    yalex_stack_pop_sp(world);	//do not print residual token
}

void token_peek_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    numeric_type* addr = out[0]->data.number;
    SP.data.number = (numeric_type) (*addr); //cast address to pointer, then deref
    SP.meta = YALEX_TOKEN_NUM;
}

void token_select_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    // since the "true" stack item is at index 1, and "false" at 0 - we can just use the bool check as index
    char truthy = ((out[2]->meta == YALEX_TOKEN_NUM && out[2]->data.number != 0)
                   || (out[2]->meta == YALEX_TOKEN_NAN && strlen(out[2]->data.text) > 0));

    SP.meta = out[truthy]->meta;
    
    if (SP.meta == YALEX_TOKEN_NUM) {
        SP.data.number = out[truthy]->data.number;
    } else if (SP.meta == YALEX_TOKEN_LAMBDA_DEFERRED) {
        SP.meta = YALEX_TOKEN_LAMBDA;
        char *target = SP.data.text;
        char *src = &out[truthy]->data.text[1];
        strcpy_s(target, YALEX_SIZE_TOKEN_STR, src);
    } else {
        char *target = SP.data.text;
        char *src = out[truthy]->data.text;
        strcpy_s(target, YALEX_SIZE_TOKEN_STR, src);
    }
}

void token_dup_helper(yalex_world *world, stack_item *out) {
    SP.meta = out->meta;
    if (SP.meta == YALEX_TOKEN_NUM) {
        SP.data.number = out->data.number;
    } else {
        strcpy_s(SP.data.text, YALEX_SIZE_TOKEN_STR, out->data.text);
    }
}

void token_pop_exec(yalex_world *world, stack_item **out) {
    yalex_stack_pop_sp(world);
}

void token_push_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
}

void token_dup_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    token_dup_helper(world, out[0]);
    yalex_stack_push_sp(world);
    token_dup_helper(world, out[0]);
}

void token_resolve_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    SP.meta = YALEX_TOKEN_LAMBDA;
    strcpy_s(SP.data.text, YALEX_SIZE_TOKEN_STR, &SP.data.text[1]);
}

void token_dump_exec(yalex_world *world, stack_item **out) {
    yalex_print_str(world, "DUMP\nSTACK:");
    for (unsigned char i = 1; i <= world->sp; i++) {
        yalex_print_token(world, &world->stack[i]);
    }
    yalex_print_str(world, "LAMBDAS:");
    for (unsigned char i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
        if (world->lambdas[i].name[0] == 0) continue;
        #define MICROLANG_PRE_AND_POST_SIZE__TEMP 4
        #define MICROLANG_BUF_SIZE__TEMP (YALEX_SIZE_TOKEN_STR + YALEX_SIZE_LAMBDA_STACK_STR + MICROLANG_PRE_AND_POST_SIZE__TEMP)
        const char * prefix = "=>(";
        const char * postfix = ")";
        char buf[MICROLANG_BUF_SIZE__TEMP];
        buf[0] = 0;
        strcat_s(buf, MICROLANG_BUF_SIZE__TEMP, world->lambdas[i].name);
        strcat_s(buf, MICROLANG_BUF_SIZE__TEMP, prefix);
        strcat_s(buf, MICROLANG_BUF_SIZE__TEMP, world->lambdas[i].stack);
        strcat_s(buf, MICROLANG_BUF_SIZE__TEMP, postfix);
        yalex_print_str(world, buf);
        #undef MICROLANG_BUF_SIZE__TEMP
        #undef MICROLANG_PRE_AND_POST_SIZE__TEMP
    }
    yalex_print_str(world, "REGISTERS:");
    char bufTotal[34 * YALEX_SIZE_REGISTERS];
    bufTotal[0] = 0;
    for (unsigned char i = 0; i < YALEX_SIZE_REGISTERS; i++) {
        char buf[32]; //TODO: which length for long long or x?
        YALEXNTOA(world->registers[i], buf, 32, 10);
        strcat_s(bufTotal, 34 * YALEX_SIZE_REGISTERS, buf);
        strcat_s(bufTotal, 34 * YALEX_SIZE_REGISTERS, ", ");
    }
    yalex_print_str(world, bufTotal);
    yalex_print_str(world, "/DUMP");
}

void token_regset_exec(yalex_world *world, stack_item **out) {
    numeric_type idx = out[0]->data.number;
    if (idx < YALEX_SIZE_REGISTERS && idx >= 0) {
        world->registers[idx] = SP.data.number;
    } else {
        int x = 0;
    }
}

void token_regget_exec(yalex_world *world, stack_item **out) {
    char *name = world->stack[world->sp + 1].data.text;
    unsigned char idx = name[1] - '0';
    if (idx < YALEX_SIZE_REGISTERS && idx >= 0) {
        yalex_stack_push_sp(world);
        SP.meta = YALEX_TOKEN_NUM;
        SP.data.number = world->registers[idx];
        //microlang_push_sp(world);
    } else {
        int x = 0;
    }
}