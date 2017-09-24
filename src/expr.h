/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef EXPR_EXPR_H
#define EXPR_EXPR_H 1

#define is_expr_blank(b) ( (b) == ' ' || (b) == '\n' || (b) == '\r' || (b) == '\t' )

#define is_expr_op(b) ( (b) == '+' || (b) == '-' || (b) == '*' || (b) == '/')

int expr_get_op_precedence(char op);

#endif
