/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "expr.h"
#include "memory.h"
#include "stack.h"
#include <stdlib.h>
#include <ctype.h>
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

static int expr_is_valid_number(const char *num, const size_t num_size);

int expr_get_op_precedence(const char op) {
    if (op == '+' || op == '-') {
        return 0;
    }

    if (op == '*' || op == '/') {
        return 1;
    }

    return -1;
}

static int expr_is_valid_number(const char *num, const size_t num_size) {
    const char *np, *np_end;

    if (num == NULL) {
        return 0;
    }

    np = num;
    np_end = np + num_size;

    if (*np == '-' && (np + 1) == np_end) {
        return 0;
    }

    np += (*np == '-');

    while (np != np_end) {
        if (!isdigit(*np)) {
            return 0;
        }

        np++;
    }

    return 1;
}

char *expr_get_curr_symbol(const char *buffer, const char *buffer_end, const char **next, size_t *symbol_size) {
    const char *bp, *tbp;
    char *symbol = NULL;
    int neg = 0;

    if (buffer == NULL || *buffer == 0 || buffer_end == NULL ||
        next == NULL || symbol_size == NULL || buffer >= buffer_end) {
        if (symbol_size != NULL) {
            *symbol_size = 0;
        }
        return NULL;
    }

    *symbol_size = 0;

    // INFO(Rafael): Finding the initial point of the current symbol.

    bp = buffer;

    while (bp != buffer_end && is_expr_blank(*bp)) {
        bp++;
    }

    // INFO(Rafael): Finding the initial point of the next symbol.

    if (*bp == '-' && (bp + 1) != buffer_end && isdigit(*(bp + 1))) {
        bp += 1;
        neg = 1;
    }

    if (isdigit(*bp)) {
        (*next) = bp + 1;

        while ((*next) != buffer_end && isdigit(**next)) {
            (*next)++;
        }

        bp -= neg;
    } else if (is_expr_op(*bp) || (*bp) == '(' || (*bp) == ')') {
        (*next) = bp + 1;
    } else {
        return NULL;
    }

    // INFO(Rafael): Copying the current symbol and returning it.

    *symbol_size = (*next) - bp;
    symbol = (char *) expr_alloc(*symbol_size + 1);
    memset(symbol, 0, *symbol_size + 1);
    memcpy(symbol, bp, *symbol_size);

    return symbol;
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

int expr_check(const char *expr, const size_t expr_size) {
    const char *ep_next, *ep_end;
    char *symbol = NULL;
    size_t symbol_size;
    int is_valid;
    int state = 0;
    int p_nr = 0;

    if (expr == NULL || expr_size == 0) {
        return 0;
    }

    ep_next = expr;
    ep_end = ep_next + expr_size;

    is_valid = 1;

    symbol = expr_get_curr_symbol(ep_next, ep_end, &ep_next, &symbol_size);

    while (symbol != NULL && is_valid) {
        if (symbol_size == 1 && !isdigit(*symbol)) {
            if (*symbol == '(') {
                p_nr++;
            } else if (*symbol == ')') {
                p_nr--;
                is_valid = (p_nr > -1);
            } else {
                is_valid = (expr_get_op_precedence(*symbol) > -1);
                state = 1;
            }
        } else if (symbol_size > 1) {
            is_valid = expr_is_valid_number(symbol, symbol_size);
            state = 0;
        } else {
            state = 0;
        }

        expr_free(symbol);

        if (is_valid) {
            symbol = expr_get_curr_symbol(ep_next, ep_end, &ep_next, &symbol_size);
        }
    }

    return (is_valid && state == 0 && p_nr == 0);
}

#undef expr_op_prologue

#undef expr_op_ab_text

#undef expr_op_epilogue
