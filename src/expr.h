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

int expr_get_op_precedence(char op);

int expr_add(expr_stack_ctx **stack, int *has_error);

int expr_sub(expr_stack_ctx **stack, int *has_error);

int expr_mul(expr_stack_ctx **stack, int *has_error);

int expr_div(expr_stack_ctx **stack, int *has_error);

#endif
