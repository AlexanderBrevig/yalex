#ifdef __cplusplus
extern "C" {
#endif

#include "yalex_tokens.h"
#include "yalex_parse.h"

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
    numeric_type* addr = (numeric_type*) out[0]->data.number;
    SP.data.number = (numeric_type) (*addr); //cast address to pointer, then deref
    SP.meta = YALEX_TOKEN_NUM;
}

void token_select_exec(yalex_world *world, stack_item **out) {
    yalex_stack_push_sp(world);
    // since the "true" stack item is at index 1, and "false" at 0 - we can just use the bool check as index
    char truthy = ((out[2]->meta == YALEX_TOKEN_NUM && out[2]->data.number != 0)
                    || (out[2]->meta == YALEX_TOKEN_NAN && YALEX_STRLEN(out[2]->data.text) > 0));
    #ifdef YALEX_DO_NOT_RESERVE_MEMORY
    if (SP.meta == YALEX_TOKEN_NUM && out[truthy]->meta != YALEX_TOKEN_NUM) {
        SP.data.text = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
    }
    #endif
    SP.meta = out[truthy]->meta;

    if (SP.meta == YALEX_TOKEN_NUM) {
        SP.data.number = out[truthy]->data.number;
    } else if (SP.meta == YALEX_TOKEN_LAMBDA_DEFERRED) {
        SP.meta = YALEX_TOKEN_LAMBDA;
        char *target = SP.data.text;
        char *src = &out[truthy]->data.text[1];
        YALEX_STRCPY(target, YALEX_SIZE_TOKEN_STR, src);
    } else {
        char *target = SP.data.text;
        char *src = out[truthy]->data.text;
        YALEX_STRCPY(target, YALEX_SIZE_TOKEN_STR, src);
    }
}

void token_dup_helper(yalex_world *world, stack_item *out) {
    SP.meta = out->meta;
    if (SP.meta == YALEX_TOKEN_NUM) {
        SP.data.number = out->data.number;
    } else {
        YALEX_STRCPY(SP.data.text, YALEX_SIZE_TOKEN_STR, out->data.text);
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
    YALEX_STRCPY(SP.data.text, YALEX_SIZE_TOKEN_STR, &SP.data.text[1]);
}

void token_dump_exec(yalex_world *world, stack_item **out) {
    yalex_print_str(world, "DUMP:");
    for (unsigned char i = 1; i <= world->sp; i++) {
        yalex_print_token(world, &world->stack[i]);
    }
    yalex_print_str(world, "LAMBDAS:");
    for (unsigned char i = 0; i < YALEX_SIZE_LAMBDAS_STACK; i++) {
        if (world->lambdas[i].name[0] == 0) continue;
        #define YALEX_PRE_AND_POST_SIZE__TEMP 4
        #define YALEX_BUF_SIZE__TEMP (YALEX_SIZE_TOKEN_STR + YALEX_SIZE_LAMBDA_STACK_STR + YALEX_PRE_AND_POST_SIZE__TEMP)
        const char * prefix = "=>(";
        const char * postfix = ")";
        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        char buf[YALEX_BUF_SIZE__TEMP];
        #else
        char *buf = (char*) YALEX_MALLOC(YALEX_BUF_SIZE__TEMP);
        #endif
        buf[0] = 0;
        YALEX_STRCAT(buf, YALEX_BUF_SIZE__TEMP, world->lambdas[i].name);
        YALEX_STRCAT(buf, YALEX_BUF_SIZE__TEMP, prefix);
        YALEX_STRCAT(buf, YALEX_BUF_SIZE__TEMP, world->lambdas[i].stack);
        YALEX_STRCAT(buf, YALEX_BUF_SIZE__TEMP, postfix);
        yalex_print_str(world, buf);
        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        YALEX_FREE(buf);
        #endif
        #undef YALEX_BUF_SIZE__TEMP
        #undef YALEX_PRE_AND_POST_SIZE__TEMP
    }
    yalex_print_str(world, "REGISTERS:");
    for (unsigned char i = 0; i < YALEX_SIZE_REGISTERS; i++) {
        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        char buf[YALEX_SIZE_TOKEN_STR];
        char print[YALEX_SIZE_TOKEN_STR + 10];
        #else
        char *buf = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
        char *print = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR + 10);
        #endif
        buf[0] = 0;
        print[0] = 0;
        YALEX_STRCAT(print, YALEX_SIZE_TOKEN_STR + 4, "R");
        YALEX_NUM_TO_STR((numeric_type) i, buf, 10);
        YALEX_STRCAT(print, YALEX_SIZE_TOKEN_STR + 4, buf);
        YALEX_STRCAT(print, YALEX_SIZE_TOKEN_STR + 4, ":\t");
        YALEX_NUM_TO_STR(world->registers[i], buf, 10);
        YALEX_STRCAT(print, YALEX_SIZE_TOKEN_STR + 4, buf);

        yalex_print_str(world, print);
        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        YALEX_FREE(buf);
        YALEX_FREE(print);
        #endif
    }
    yalex_print_str(world, "/DUMP");
}
void token_reg_err(yalex_world *world, numeric_type idx) {
    char buf[22];
    buf[0] = 0;
    YALEX_STRCAT(buf, 22, "No such register ");
    char nbuf[4];
    YALEX_NUM_TO_STR(idx, nbuf, 10);
    YALEX_STRCAT(buf, 22, nbuf);
    yalex_print_err(world, buf);
}
void token_regset_exec(yalex_world *world, stack_item **out) {
    numeric_type idx = out[0]->data.number;
    if (idx < YALEX_SIZE_REGISTERS && idx >= 0) {
        world->registers[idx] = SP.data.number;
    } else {
        token_reg_err(world, idx);
    }
}

void token_regget_exec(yalex_world *world, stack_item **out) {
    char *name = world->stack[world->sp + 1].data.text;
    unsigned char idx = name[1] - '0';
    if (idx < YALEX_SIZE_REGISTERS && idx >= 0) {
        yalex_stack_push_sp(world);
        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        if (SP.meta == YALEX_TOKEN_NAN) {
            YALEX_FREE(SP.data.text);
        }
        #endif
        SP.meta = YALEX_TOKEN_NUM;
        SP.data.number = world->registers[idx];
    } else {
        token_reg_err(world, idx);
    }
}

void token_clr_exec(yalex_world *world, stack_item **out) {
    yalex_init(world, world->onResultCallback);
}

void token_run_exec(yalex_world *world, stack_item **out) {
    char *prog = out[0]->data.text;
    if (prog[0] == '"') {
        prog++;
        prog[YALEX_STRLEN(prog) - 1] = 0;
        #ifndef YALEX_DO_NOT_RESERVE_MEMORY
        char buf[YALEX_SIZE_TOKEN_STR];
        #else
        char *buf = (char*) YALEX_MALLOC(YALEX_SIZE_TOKEN_STR);
        #endif
        YALEX_STRCPY(buf, YALEX_SIZE_TOKEN_STR, prog);
        yalex_parse(world, buf);
        #ifdef YALEX_DO_NOT_RESERVE_MEMORY
        YALEX_FREE(buf);
        #endif
    }
}
#ifdef __cplusplus
}
#endif