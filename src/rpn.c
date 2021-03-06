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

    symbol = expr_get_curr_symbol(ifx_next, ifx_end, &ifx_next, &symbol_size);

    while (symbol != NULL) {
        if (symbol_size > 1 || isdigit(*symbol)) {
            // INFO(Rafael): It is a number so just enqueue it.
            rpn_enqueue(rp, rp_end, symbol, symbol_size);
        } else if (*symbol == '(') {  // INFO(Rafael): It is an operator, '(' or ')'.
            stack = expr_stack_push(stack, symbol, symbol_size);
        } else if (*symbol == ')') {
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

        expr_free(symbol);
        symbol = expr_get_curr_symbol(ifx_next, ifx_end, &ifx_next, &symbol_size);
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

        rpn = expr_realloc(rpn, rp_size + 1);
    }

    return rpn;
}

int expr_eval(const char *rpn, const size_t rpn_size, int *has_error) {
    const char *rp_next, *rp_end;
    expr_stack_ctx *stack = NULL;
    char *symbol;
    size_t symbol_size;
    int result = 0;

    if (rpn == NULL || rpn_size == 0 || has_error == NULL) {
        return 0;
    }

    rp_next = rpn;
    rp_end = rp_next + rpn_size;

    symbol = expr_get_curr_symbol(rp_next, rp_end, &rp_next, &symbol_size);

    *has_error = 0;

    while (symbol != NULL) {
        if (symbol_size > 1 || isdigit(*symbol)) {
            stack = expr_stack_push(stack, symbol, symbol_size);
        } else if (is_expr_op(*symbol)) {
            switch (*symbol) {
                case '+':
                    expr_add(&stack, has_error);
                    break;

                case '-':
                    expr_sub(&stack, has_error);
                    break;

                case '*':
                    expr_mul(&stack, has_error);
                    break;

                case '/':
                    expr_div(&stack, has_error);
                    break;

                default:
                    // WARN(Rafael): It should never happen.
                    *has_error = 1;
                    printf("WARN: unexpected operator '%c'.\n", *symbol);
                    expr_free(symbol);
                    return 0;
            }
        }

        expr_free(symbol);

        if (*has_error) {
            return 0;
        }

        symbol = expr_get_curr_symbol(rp_next, rp_end, &rp_next, &symbol_size);
    }

    if (expr_stack_empty(stack)) {
        // WARN(Rafael): Some error occurred.
        *has_error = 1;
        return 0;
    }

    result = atoi(expr_stack_top(stack)->data);

    stack = expr_stack_pop(stack);

    if (!expr_stack_empty(stack)) {
        // WARN(Rafael): In cases of well converted expressions it should never happen.
        //               But a memory leak is pretty ugly.
        expr_stack_free(stack);
        *has_error = 1;
    }

    return result;
}

#undef rpn_enqueue

#undef rpn_enqueue_and_pop
