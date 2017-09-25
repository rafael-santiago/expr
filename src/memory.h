/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_MEMORY_H
#define EXPR_MEMORY_H 1

#include <stdlib.h>

void *expr_alloc(const size_t segsize);

void *expr_realloc(void *maddr, const size_t segsize);

void expr_free(void *maddr);

#endif
