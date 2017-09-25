/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_STACK_H
#define EXPR_STACK_H 1

#include "types.h"

expr_stack_ctx *expr_stack_push(expr_stack_ctx *stack, const char *data, const size_t data_size);

expr_stack_ctx *expr_stack_pop(expr_stack_ctx *stack);

#define expr_stack_top(s) ( (s) == NULL ? NULL : (s) )

#define expr_stack_empty(s) ( (s) == NULL )

void expr_stack_free(expr_stack_ctx *stack);

#endif
