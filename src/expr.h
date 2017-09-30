/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_EXPR_H
#define EXPR_EXPR_H 1

#include "types.h"

#define is_expr_blank(b) ( (b) == ' ' || (b) == '\n' || (b) == '\r' || (b) == '\t' )

#define is_expr_op(b) ( (b) == '+' || (b) == '-' || (b) == '*' || (b) == '/')

#define expr_operator_ab_decl(opname, stack, has_error) void expr_ ## opname (expr_stack_ctx **stack, int *has_error);

int expr_get_op_precedence(char op);

char *expr_get_curr_symbol(const char *buffer, const char *buffer_end, const char **next, size_t *symbol_size);

expr_operator_ab_decl(add, stack, has_error);

expr_operator_ab_decl(sub, stack, has_error);

expr_operator_ab_decl(mul, stack, has_error);

expr_operator_ab_decl(div, stack, has_error);

int expr_check(const char *expr, const size_t expr_size);

#undef expr_operator_ab_decl

#endif
