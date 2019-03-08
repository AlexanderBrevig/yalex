
#include <string.h>

#include "yalex_system.h"
#include "yalex_tokens.h"

yalex_sys *yalex_system() {
    static yalex_sys system;
    return &system;
}

char yalex_system_token_register(yalex_world *world, const char* token, char *requirements, char requirementCount, tokenExec exec) {
    if (yalex_system()->tokenCount + 1 >= YALEX_SIZE_TOKENS) return 0;
    yalex_system()->tokens[yalex_system()->tokenCount].token = token;
    yalex_system()->tokens[yalex_system()->tokenCount].requirementCount = requirementCount;
    for (int i = 0; i < YALEX_SIZE_MAX_DEPENDABLE_STACK; i++) {
        yalex_system()->tokens[yalex_system()->tokenCount].requirements[i] = -1;
    }
    if (requirements) {
        memcpy(yalex_system()->tokens[yalex_system()->tokenCount].requirements, requirements, requirementCount);
    }
    yalex_system()->tokens[yalex_system()->tokenCount].exec = exec;
    yalex_system()->tokenCount++;
    return yalex_system()->tokenCount;
}

void yalex_system_lambda_init(lambda *lm, const char* name, const char* stack) {
    strcpy_s(lm->name, YALEX_SIZE_TOKEN_STR, name);
    strcpy_s(lm->stack, YALEX_SIZE_TOKEN_STR, stack);
}

void yalex_system_init(yalex_world *world) {
    /// Configure tokens
    char basic_op_req[2] = { YALEX_TOKEN_NUM, YALEX_TOKEN_NUM };
    yalex_system_token_register(world, "+", basic_op_req, 2, token_add_exec);
    yalex_system_token_register(world, "-", basic_op_req, 2, token_min_exec);
    yalex_system_token_register(world, "*", basic_op_req, 2, token_mul_exec);
    yalex_system_token_register(world, "/", basic_op_req, 2, token_div_exec);
    yalex_system_token_register(world, "<", basic_op_req, 2, token_lt_exec);
    yalex_system_token_register(world, ">", basic_op_req, 2, token_gt_exec);
    yalex_system_token_register(world, "==", basic_op_req, 2, token_eq_exec);
    yalex_system_token_register(world, "!=", basic_op_req, 2, token_neq_exec);
    yalex_system_token_register(world, "||", basic_op_req, 2, token_or_exec);
    yalex_system_token_register(world, "&&", basic_op_req, 2, token_and_exec);
    yalex_system_token_register(world, "print", 0, 0, token_print_exec);
    yalex_system_token_register(world, "peek", basic_op_req, 1, token_peek_exec);
    yalex_system_token_register(world, "select", 0, 3, token_select_exec);
    //microlang_token_register(world, "pop", 0, 0, token_pop_exec);
    //microlang_token_register(world, "push", 0, 0, token_push_exec);
    yalex_system_token_register(world, "dup", 0, 1, token_dup_exec);

    char basic_resolve_req[1] = { YALEX_TOKEN_LAMBDA_DEFERRED };
    yalex_system_token_register(world, "resolve", basic_resolve_req, 1, token_resolve_exec);
    
    yalex_system_token_register(world, "dump", 0, 0, token_dump_exec);
    //yalex_system_token_register(world, "R*S", basic_op_req, 1, token_regset_exec);
    yalex_system_token_register(world, "R*R", 0, 0, token_regget_exec);

    /// Configure lambdas

    for (size_t i = 0; i < YALEX_SIZE_SYS_LAMBDAS_STACK; i++) {
        yalex_lambda_init(yalex_system()->lambdas[i]);
    }
    yalex_system_lambda_init(&yalex_system()->lambdas[0], "nop", "()");
    yalex_system_lambda_init(&yalex_system()->lambdas[1], "_", "('nop)");
}