/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "expr.h"

int expr_get_op_precedence(char op) {
    if (op == '+' || op == '-') {
        return 0;
    }

    if (op == '*' || op == '/') {
        return 1;
    }

    return -1;
}
