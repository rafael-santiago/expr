/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "rpn.h"
#include "stack.h"
#include "expr.h"
#include "memory.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define rpn_enqueue(queue, queue_end, data, data_size) {\
    if ((queue + data_size) >= queue_end) {\
        printf("ERROR: Expression buffer is too long.\n");\
        exit(1);\
    }\
    memcpy(queue, data, data_size);\
    queue += data_size;\
    if ((queue + 2) >= queue_end) {\
        printf("ERROR: Expression buffer is too long.\n");\
        exit(1);\
    }\
    *queue = ' ';\
    queue += 1;\
}

#define rpn_enqueue_and_pop(stack, top, queue, queue_end) {\
    rpn_enqueue(queue, queue_end, top->data, top->data_size);\
    stack = expr_stack_pop(stack);\
    top = expr_stack_top(stack);\
}\

static char *get_curr_symbol(const char *buffer, const char *buffer_end, const char **next, size_t *symbol_size);

static char *get_curr_symbol(const char *buffer, const char *buffer_end, const char **next, size_t *symbol_size) {
    const char *bp, *tbp;
    char *symbol = NULL;

    if (buffer == NULL || *buffer == 0 || buffer_end == NULL ||
        next == NULL || symbol_size == NULL || buffer >= buffer_end) {
        return NULL;
    }

    *symbol_size = 0;

    // INFO(Rafael): Finding the initial point of the current symbol.

    bp = buffer;

    while (bp != buffer_end && is_expr_blank(*bp)) {
        bp++;
    }

    // INFO(Rafael): Finding the initial point of the next symbol.

    if (isdigit(*bp)) {
        (*next) = bp + 1;

        while ((*next) != buffer_end && isdigit(**next)) {
            (*next)++;
        }
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

char *expr_ifx2rpn(const char *ifx, const size_t ifx_size, size_t *rpn_size) {
    char *rpn, *rp, *rp_end;
    const char *ifx_next, *ifx_end;
    size_t rp_size;
    char *symbol;
    size_t symbol_size;
    expr_stack_ctx *stack = NULL, *top;
    int curr_op_precedence;

    if (ifx == NULL || ifx_size == 0) {
        return NULL;
    }

    // INFO(Rafael): 64kb buffer as default output.
    rp_size = 65535;

    rpn = (char *) expr_alloc(rp_size);
    memset(rpn, 0, rp_size);

    rp = rpn;
    rp_end = rp + rp_size;

    ifx_next = ifx;
    ifx_end = ifx + ifx_size;

    symbol = get_curr_symbol(ifx_next, ifx_end, &ifx_next, &symbol_size);

    while (symbol != NULL) {
        if (symbol_size > 1 || isdigit(*symbol)) {
            // INFO(Rafael): It is a number so just enqueue it.
            rpn_enqueue(rp, rp_end, symbol, symbol_size);
        } else {
            // INFO(Rafael): It is an operator or ).
            if (strcmp(symbol, "(") == 0) {
                stack = expr_stack_push(stack, symbol, symbol_size);
            } else if (strcmp(symbol, ")") == 0) {
                // INFO(Rafael): Pop from stack and enqueue into rpn until a '(' is gotten.
                top = expr_stack_top(stack);
                while (!expr_stack_empty(stack) && strcmp(top->data, "(") != 0) {
                    rpn_enqueue_and_pop(stack, top, rp, rp_end);
                }
                stack = expr_stack_pop(stack);
            } else if (is_expr_op(*symbol)) {
                // INFO(Rafael): Pop from stack while there is on the top of it an operator with precedence equal or greater
                //               than the current operator. However, before poping enqueue it in rpn.

                top = expr_stack_top(stack);
                curr_op_precedence = expr_get_op_precedence(*symbol);
                while (!expr_stack_empty(stack) && is_expr_op(*top->data) &&
                       expr_get_op_precedence(*top->data) >= curr_op_precedence) {
                    rpn_enqueue_and_pop(stack, top, rp, rp_end);
                }

                // INFO(Rafael): Push onto stack the current operator.
                stack = expr_stack_push(stack, symbol, symbol_size);
            }
        }

        expr_free(symbol);
        symbol = get_curr_symbol(ifx_next, ifx_end, &ifx_next, &symbol_size);
    }

    top = expr_stack_top(stack);
    while (!expr_stack_empty(stack)) {
        rpn_enqueue_and_pop(stack, top, rp, rp_end);
    }

    for (rp = rpn; *rp != 0; rp++)
        ;

    rp_size = rp - rpn;

    if (rpn[rp_size - 1] == ' ') {
        // WARN(Rafael): It could be done without the previous if clause but let's make sure.
        rpn[rp_size - 1] = 0;
        rp_size -= 1;
    }

    if (rpn_size != NULL) {
        // INFO(Rafael): If the user has passed a rpn_size pointer we can adjust the output size, probably
        //               saving memory.

        *rpn_size = rp_size;

        rpn = expr_realloc(rpn, rp_size);
    }

    return rpn;
}

#undef rpn_enqueue

#undef rpn_enqueue_and_pop