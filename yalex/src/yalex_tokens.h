#ifndef YALEX_TOKENS_H
#define YALEX_TOKENS_H

#include "yalex.h"

void token_add_exec(yalex_world *world, stack_item **out);
void token_min_exec(yalex_world *world, stack_item **out);
void token_mul_exec(yalex_world *world, stack_item **out);
void token_div_exec(yalex_world *world, stack_item **out);
void token_lt_exec(yalex_world *world, stack_item **out);
void token_gt_exec(yalex_world *world, stack_item **out);
void token_eq_exec(yalex_world *world, stack_item **out);
void token_neq_exec(yalex_world *world, stack_item **out);
void token_or_exec(yalex_world *world, stack_item **out);
void token_and_exec(yalex_world *world, stack_item **out);
void token_print_exec(yalex_world *world, stack_item **out);
void token_peek_exec(yalex_world *world, stack_item **out);
void token_select_exec(yalex_world *world, stack_item **out);
void token_dup_helper(yalex_world *world, stack_item *out);
void token_pop_exec(yalex_world *world, stack_item **out);
void token_push_exec(yalex_world *world, stack_item **out);
void token_dup_exec(yalex_world *world, stack_item **out);
void token_resolve_exec(yalex_world *world, stack_item **out);
void token_dump_exec(yalex_world *world, stack_item **out);
void token_regset_exec(yalex_world *world, stack_item **out);
void token_regget_exec(yalex_world *world, stack_item **out);
void token_clr_exec(yalex_world *world, stack_item **out);
void token_run_exec(yalex_world *world, stack_item **out);

#endif