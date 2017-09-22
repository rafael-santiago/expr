#include "stack.h"
#include "memory.h"
#include <string.h>

expr_stack_ctx *expr_stack_push(expr_stack_ctx *stack, const char *data, const size_t data_size) {
    expr_stack_ctx *new_top = NULL;

    if (data == NULL || data_size == 0) {
        return stack;
    }

    new_top = (expr_stack_ctx *) expr_alloc(sizeof(expr_stack_ctx));

    new_top->data = (char *) expr_alloc(data_size + 1);
    memset(new_top->data, 0, data_size + 1);
    memcpy(new_top->data, data, data_size);
    new_top->data_size = data_size;

    new_top->next = stack;

    return new_top;
}

expr_stack_ctx *expr_stack_pop(expr_stack_ctx *stack) {
    expr_stack_ctx *new_top;

    if (stack == NULL) {
        return NULL;
    }

    new_top = stack->next;

    expr_free(stack->data);
    expr_free(stack);

    return new_top;
}
