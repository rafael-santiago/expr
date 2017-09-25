/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_RPN_H
#define EXPR_RPN_H 1

#include <stdlib.h>

char *expr_ifx2rpn(const char *ifx, const size_t ifx_size, size_t *rpn_size);

int expr_eval(const char *rpn, const size_t rpn_size, int *has_error);

#endif
