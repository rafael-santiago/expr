/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_TYPES_H
#define EXPR_TYPES_H 1

#include <stdlib.h>

#define EXPR_VERSION "0.1"

typedef struct expr_stack {
    char *data;
    size_t data_size;
    struct expr_stack *next;
}expr_stack_ctx;

#endif
