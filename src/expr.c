/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "expr.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define expr_op_prologue(stack, has_error) {\
    if (has_error == NULL) {\
        return 0;\
    }\
    if (stack == NULL) {\
        *has_error = 1;\
        return 0;\
    }\
    *has_error = 0;\
}

#define expr_op_ab_text(stack, a, b, has_error) {\
    if (expr_stack_empty(*stack)) {\
        *has_error = 1;\
        return 0;\
    }\
    a = atoi(expr_stack_top(*stack)->data);\
    (*stack) = expr_stack_pop(*stack);\
    if (expr_stack_empty(*stack)) {\
        *has_error = 1;\
        return 0;\
    }\
    b = atoi(expr_stack_top(*stack)->data);\
    (*stack) = expr_stack_pop(*stack);\
}\

#define expr_op_epilogue(stack, has_error, tmp_buf, result) {\
    sprintf(tmp, "%d", a);\
    (*stack) = expr_stack_push(*stack, tmp, strlen(tmp));\
    if ((*stack) == NULL) {\
        *has_error = 1;\
        return 0;\
    }\
    return result;\
}

int expr_get_op_precedence(char op) {
    if (op == '+' || op == '-') {
        return 0;
    }

    if (op == '*' || op == '/') {
        return 1;
    }

    return -1;
}

int expr_add(expr_stack_ctx **stack, int *has_error) {
    int a, b;
    expr_stack_ctx *top = NULL;
    char tmp[255];

    expr_op_prologue(stack, has_error);
    expr_op_ab_text(stack, a, b, has_error);
    a += b;
    expr_op_epilogue(stack, has_error, tmp, a);
}

int expr_sub(expr_stack_ctx **stack, int *has_error) {
    int a, b;
    expr_stack_ctx *top = NULL;
    char tmp[255];

    expr_op_prologue(stack, has_error);
    expr_op_ab_text(stack, a, b, has_error);
    a -= b;
    expr_op_epilogue(stack, has_error, tmp, a);
}

int expr_mul(expr_stack_ctx **stack, int *has_error) {
    int a, b;
    expr_stack_ctx *top = NULL;
    char tmp[255];

    expr_op_prologue(stack, has_error);
    expr_op_ab_text(stack, a, b, has_error);
    a *= b;
    expr_op_epilogue(stack, has_error, tmp, a);
}

int expr_div(expr_stack_ctx **stack, int *has_error) {
    int a, b;
    expr_stack_ctx *top = NULL;
    char tmp[255];

    expr_op_prologue(stack, has_error);
    expr_op_ab_text(stack, a, b, has_error);
    a /= b;
    expr_op_epilogue(stack, has_error, tmp, a);
}

#undef expr_op_prologue

#undef expr_op_ab_text

#undef expr_op_epilogue
