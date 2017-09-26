/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "types.h"
#include "expr.h"
#include "rpn.h"
#include "memory.h"
#include <string.h>
#include <signal.h>
#include <stdio.h>

static int expr_exit = 0;

static void sigint_watchdog(int signr);

int main(int argc, char **argv) {
    char *rpn, ifx_expr[255];
    size_t rpn_size;
    int result, has_error;

    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0) {
            printf("expr-v%s\n\n", EXPR_VERSION);
            printf("expr is Copyright (C) 2015-2017 by Rafael Santiago.\n"
                   "Bug reports, feedback, etc: <voidbrainvoid@gmail.com> or "
                   "<https://github.com/rafael-santiago/expr/issues>\n");
            return 0;
        }
    }

    signal(SIGINT, sigint_watchdog);
    signal(SIGTERM, sigint_watchdog);

    printf("INFO: to exit the program type <CTRL>+c and then <ENTER>.\n\n");

    while (!expr_exit) {
        printf(">>> ");
        memset(ifx_expr, 0, sizeof(ifx_expr) / sizeof(ifx_expr[0]));
        fgets(ifx_expr, sizeof(ifx_expr) / sizeof(ifx_expr[0]), stdin);
        if (expr_exit || (strlen(ifx_expr) == 1 && ifx_expr[0] == '\n')) {
            continue;
        }
        rpn = NULL;
        if (expr_check(ifx_expr, strlen(ifx_expr))) {
            rpn = expr_ifx2rpn(ifx_expr, strlen(ifx_expr), &rpn_size);
        }
        if (rpn != NULL) {
            result = expr_eval(rpn, rpn_size, &has_error);
            if (!has_error) {
                printf("%d\n", result);
            } else {
                printf("ERROR: during expression evaluation.\n");
            }
            expr_free(rpn);
        } else {
            printf("ERROR: invalid expression.\n");
        }
    }

    return 1;
}

static void sigint_watchdog(int signr) {
    expr_exit = 1;
}
