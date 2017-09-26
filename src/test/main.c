/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include <cutest.h>
#include "../stack.h"
#include "../memory.h"
#include "../expr.h"
#include "../rpn.h"
#include <string.h>

CUTE_TEST_CASE(memory_tests)
    char *data = NULL;
    data = expr_alloc(10);
    CUTE_ASSERT(data != NULL);
    // INFO(Rafael): No SIGSEGV must happen. If "data" was not properly freed, the memory leak check
    //               system from unit test library will detect this issue.
    expr_free(data);


    // INFO(Rafael): If the realloc is broken it will result in memory invalid access and you will know.

    data = expr_alloc(2);
    CUTE_ASSERT(data != NULL);
    memcpy(data, "a", 1);
    data = expr_realloc(data, 1024);
    CUTE_ASSERT(*data == 'a');
    CUTE_ASSERT(data != NULL);
    memset(data, 0, 1024);

    expr_free(data);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_stack_ctx_tests)
    expr_stack_ctx *stack = NULL, *top = NULL;

    CUTE_ASSERT(expr_stack_empty(stack) == 1);

    stack = expr_stack_push(stack, "abc", 3);
    CUTE_ASSERT(stack != NULL);

    top = expr_stack_top(stack);
    CUTE_ASSERT(top != NULL);
    CUTE_ASSERT(top->data != NULL);
    CUTE_ASSERT(top->data_size == 3);
    CUTE_ASSERT(strcmp(top->data, "abc") == 0);

    stack = expr_stack_push(stack, "def", 3);
    CUTE_ASSERT(stack != NULL);

    top = expr_stack_top(stack);
    CUTE_ASSERT(top != NULL);
    CUTE_ASSERT(top->data != NULL);
    CUTE_ASSERT(top->data_size == 3);
    CUTE_ASSERT(strcmp(top->data, "def") == 0);


    stack = expr_stack_push(stack, "gh", 2);
    CUTE_ASSERT(stack != NULL);

    top = expr_stack_top(stack);
    CUTE_ASSERT(top != NULL);
    CUTE_ASSERT(top->data != NULL);
    CUTE_ASSERT(top->data_size == 2);
    CUTE_ASSERT(strcmp(top->data, "gh") == 0);

    stack = expr_stack_pop(stack);

    top = expr_stack_top(stack);
    CUTE_ASSERT(top != NULL);
    CUTE_ASSERT(top->data != NULL);
    CUTE_ASSERT(top->data_size == 3);
    CUTE_ASSERT(strcmp(top->data, "def") == 0);

    stack = expr_stack_pop(stack);

    top = expr_stack_top(stack);
    CUTE_ASSERT(top != NULL);
    CUTE_ASSERT(top->data != NULL);
    CUTE_ASSERT(top->data_size == 3);
    CUTE_ASSERT(strcmp(top->data, "abc") == 0);

    stack = expr_stack_pop(stack);

    CUTE_ASSERT(expr_stack_empty(stack) == 1);

    // INFO(Rafael): No SIGSEGV when a null stack is passed.
    stack = expr_stack_pop(stack);

    // INFO(Rafael): Any memory leak should be detected by the memory leak system from the unit test library.
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_stack_free_tests)
    expr_stack_ctx *stack = NULL;
    char *data[] = {
        "didi",
        "dede",
        "mussum",
        "zacarias"
    };
    size_t data_nr = sizeof(data) / sizeof(data[0]), d;

    for (d = 0; d < data_nr; d++) {
        stack = expr_stack_push(stack, data[d], strlen(data[d]));
    }

    CUTE_ASSERT(stack != NULL);
    expr_stack_free(stack);

    // INFO(Rafael): Any memory leak should be detected by the memory leak system from the unit test library.
CUTE_TEST_CASE_END

CUTE_TEST_CASE(is_expr_blank_tests)
    CUTE_ASSERT(is_expr_blank(' ') == 1);
    CUTE_ASSERT(is_expr_blank('\n') == 1);
    CUTE_ASSERT(is_expr_blank('\t') == 1);
    CUTE_ASSERT(is_expr_blank('\r') == 1);
    CUTE_ASSERT(is_expr_blank('0') == 0);
    CUTE_ASSERT(is_expr_blank('1') == 0);
    CUTE_ASSERT(is_expr_blank('2') == 0);
    CUTE_ASSERT(is_expr_blank('3') == 0);
    CUTE_ASSERT(is_expr_blank('4') == 0);
    CUTE_ASSERT(is_expr_blank('5') == 0);
    CUTE_ASSERT(is_expr_blank('6') == 0);
    CUTE_ASSERT(is_expr_blank('7') == 0);
    CUTE_ASSERT(is_expr_blank('8') == 0);
    CUTE_ASSERT(is_expr_blank('9') == 0);
    CUTE_ASSERT(is_expr_blank('+') == 0);
    CUTE_ASSERT(is_expr_blank('-') == 0);
    CUTE_ASSERT(is_expr_blank('*') == 0);
    CUTE_ASSERT(is_expr_blank('/') == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_get_op_precedence_tests)
    CUTE_ASSERT(expr_get_op_precedence('+') == 0);
    CUTE_ASSERT(expr_get_op_precedence('-') == 0);
    CUTE_ASSERT(expr_get_op_precedence('*') == 1);
    CUTE_ASSERT(expr_get_op_precedence('/') == 1);
    CUTE_ASSERT(expr_get_op_precedence('0') == -1);
    CUTE_ASSERT(expr_get_op_precedence('1') == -1);
    CUTE_ASSERT(expr_get_op_precedence('2') == -1);
    CUTE_ASSERT(expr_get_op_precedence('3') == -1);
    CUTE_ASSERT(expr_get_op_precedence('4') == -1);
    CUTE_ASSERT(expr_get_op_precedence('5') == -1);
    CUTE_ASSERT(expr_get_op_precedence('6') == -1);
    CUTE_ASSERT(expr_get_op_precedence('7') == -1);
    CUTE_ASSERT(expr_get_op_precedence('8') == -1);
    CUTE_ASSERT(expr_get_op_precedence('9') == -1);
    CUTE_ASSERT(expr_get_op_precedence(' ') == -1);
    CUTE_ASSERT(expr_get_op_precedence('\n') == -1);
    CUTE_ASSERT(expr_get_op_precedence('\r') == -1);
    CUTE_ASSERT(expr_get_op_precedence('\t') == -1);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_ifx2rpn_tests)
    struct ifx2rpn_test {
        char *ifx;
        char *rpn;
    };
    struct ifx2rpn_test test_vector[] = {
        { "1 + 2", "1 2 +" },
        { "-1 + 2", "-1 2 +" },
        { "((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1))", "15 7 1 1 + - / 3 * 2 1 1 + + -" },
        { "1+2", "1 2 +" },
        { "((15/(7-(1+1)))*3)-(2+(1+1))", "15 7 1 1 + - / 3 * 2 1 1 + + -" },
        { "1 +2", "1 2 +" },
        { "1+ 2", "1 2 +" },
        { "((15/ (7 -(1 +1)))* 3)-(2      +    (1\t\t\t+1\n)\r\r)", "15 7 1 1 + - / 3 * 2 1 1 + + -" },
    };
    size_t tv_nr = sizeof(test_vector) / sizeof(test_vector[0]), tv;
    char *rpn;
    size_t rpn_size;

    for (tv = 0; tv < tv_nr; tv++) {
        rpn = expr_ifx2rpn(test_vector[tv].ifx, strlen(test_vector[tv].ifx), NULL);
        CUTE_ASSERT(rpn != NULL);
        CUTE_ASSERT(strlen(rpn) == strlen(test_vector[tv].rpn));
        CUTE_ASSERT(strcmp(rpn, test_vector[tv].rpn) == 0);
        expr_free(rpn);

        rpn = expr_ifx2rpn(test_vector[tv].ifx, strlen(test_vector[tv].ifx), &rpn_size);
        CUTE_ASSERT(rpn != NULL);
        CUTE_ASSERT(rpn_size == strlen(test_vector[tv].rpn));
        CUTE_ASSERT(strcmp(rpn, test_vector[tv].rpn) == 0);
        expr_free(rpn);
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_add_tests)
    expr_stack_ctx *stack = NULL;
    int has_error;

    stack = expr_stack_push(stack, "2", 1);
    CUTE_ASSERT(stack != NULL);

    stack = expr_stack_push(stack, "3", 1);
    CUTE_ASSERT(stack != NULL);

    CUTE_ASSERT(expr_add(&stack, &has_error) == 5);
    CUTE_ASSERT(has_error == 0);
    CUTE_ASSERT(expr_stack_empty(stack) == 0);
    CUTE_ASSERT(strcmp(expr_stack_top(stack)->data, "5") == 0);

    expr_stack_free(stack);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_sub_tests)
    expr_stack_ctx *stack = NULL;
    int has_error;

    stack = expr_stack_push(stack, "4", 1);
    CUTE_ASSERT(stack != NULL);

    stack = expr_stack_push(stack, "3", 1);
    CUTE_ASSERT(stack != NULL);

    CUTE_ASSERT(expr_sub(&stack, &has_error) == 1);
    CUTE_ASSERT(has_error == 0);
    CUTE_ASSERT(expr_stack_empty(stack) == 0);
    CUTE_ASSERT(strcmp(expr_stack_top(stack)->data, "1") == 0);

    expr_stack_free(stack);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_mul_tests)
    expr_stack_ctx *stack = NULL;
    int has_error;

    stack = expr_stack_push(stack, "50", 2);
    CUTE_ASSERT(stack != NULL);

    stack = expr_stack_push(stack, "5", 1);
    CUTE_ASSERT(stack != NULL);

    CUTE_ASSERT(expr_mul(&stack, &has_error) == 250);
    CUTE_ASSERT(has_error == 0);
    CUTE_ASSERT(expr_stack_empty(stack) == 0);
    CUTE_ASSERT(strcmp(expr_stack_top(stack)->data, "250") == 0);

    expr_stack_free(stack);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_div_tests)
    expr_stack_ctx *stack = NULL;
    int has_error;

    stack = expr_stack_push(stack, "250", 3);
    CUTE_ASSERT(stack != NULL);

    stack = expr_stack_push(stack, "5", 1);
    CUTE_ASSERT(stack != NULL);

    CUTE_ASSERT(expr_div(&stack, &has_error) == 50);
    CUTE_ASSERT(has_error == 0);
    CUTE_ASSERT(expr_stack_empty(stack) == 0);
    CUTE_ASSERT(strcmp(expr_stack_top(stack)->data, "50") == 0);

    expr_stack_free(stack);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_eval_tests)
    struct expr_eval_test {
        char *rpn;
        int result;
    };
    struct expr_eval_test test_vector[] = {
        { "1 2 +", 3 },
        { "15 7 1 1 + - / 3 * 2 1 1 + + -", 5 },
        { "15 7 / 1 - 1 3 * + 2 - 1 + 1 +", 4 },
        { "-1 -1 -", 0 },
        { "-1 -1 +", -2 }
    };
    size_t tv_nr = sizeof(test_vector) / sizeof(test_vector[0]), tv;
    int has_error;

    for (tv = 0; tv < tv_nr; tv++) {
        CUTE_ASSERT(expr_eval(test_vector[tv].rpn, strlen(test_vector[tv].rpn), &has_error) == test_vector[tv].result);
        CUTE_ASSERT(has_error == 0);
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_get_curr_symbol_tests)
    const char *expr = "-1 + ( 22 - 333 ) / 4444 * 55555";
    const char *expr_next, *expr_end;
    char *symbol;
    size_t symbol_size;

    expr_next = expr;
    expr_end = expr_next + strlen(expr_next);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 2);
    CUTE_ASSERT(strcmp(symbol, "-1") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, "+") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, "(") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 2);
    CUTE_ASSERT(strcmp(symbol, "22") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, "-") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 3);
    CUTE_ASSERT(strcmp(symbol, "333") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, ")") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, "/") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 4);
    CUTE_ASSERT(strcmp(symbol, "4444") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 1);
    CUTE_ASSERT(strcmp(symbol, "*") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol != NULL);
    CUTE_ASSERT(symbol_size == 5);
    CUTE_ASSERT(strcmp(symbol, "55555") == 0);
    expr_free(symbol);

    symbol = expr_get_curr_symbol(expr_next, expr_end, &expr_next, &symbol_size);

    CUTE_ASSERT(symbol == NULL);
    CUTE_ASSERT(symbol_size == 0);
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_check_tests)
    struct expr_check_test {
        char *expr;
        int is_valid;
    };
    struct expr_check_test test_vector[] = {
        { ""                                                      ,   0 },
        { "1"                                                     ,   1 },
        { "+"                                                     ,   0 },
        { "-"                                                     ,   0 },
        { "/"                                                     ,   0 },
        { "*"                                                     ,   0 },
        { "1 + 1"                                                 ,   1 },
        { "1 + "                                                  ,   0 },
        { "1+"                                                    ,   0 },
        { "(1 + 2 + 3)"                                           ,   1 },
        { "(1 + 2 + 3"                                            ,   0 },
        { "0 + 2 + 3) / (2 + 1)"                                  ,   0 },
        { "1 + 2"                                                 ,   1 },
        { "-1 + 2"                                                ,   1 },
        { "((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1))"            ,   1 },
        { "1+2"                                                   ,   1 },
        { "((15/(7-(1+1)))*3)-(2+(1+1))"                          ,   1 },
        { "1 +2"                                                  ,   1 },
        { "((15/ (7 -(1 +1)))* 3)-(2      +    (1\t\t\t+1\n)\r\r)",   1 },
        { "(2-1)*3",                                                  1 },
        { "2%2",                                                      0 },
        { "qwert",                                                    0 },
        { "1\n"                                                     , 1 },
        { "+\n"                                                     , 0 },
        { "-\n"                                                     , 0 },
        { "/\n"                                                     , 0 },
        { "*\n\n\n"                                                 , 0 },
        { "1 + 1\n\n\n\n"                                           , 1 },
        { "1 + \n"                                                  , 0 },
        { "1+\n"                                                    , 0 },
        { "(1 + 2 + 3)\n"                                           , 1 },
        { "(1 + 2 + 3\n"                                            , 0 },
        { "0 + 2 + 3) / (2 + 1)\n"                                  , 0 },
        { "1 + 2\n"                                                 , 1 },
        { "-1 + 2\n"                                                , 1 },
        { "((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1))\n"            , 1 },
        { "1+2\n"                                                   , 1 },
        { "((15/(7-(1+1)))*3)-(2+(1+1))\n"                          , 1 },
        { "1 +2\n"                                                  , 1 },
        { "((15/ (7 -(1 +1)))* 3)-(2      +    (1\t\t\t+1\n)\r\r)\t", 1 },
        { "(2-1)*3\n",                                                1 },
        { "2%2\n",                                                    0 },
        { "qwert\n",                                                  0 }
    };
    size_t tv, tv_nr = sizeof(test_vector) / sizeof(test_vector[0]);
    char *ep;

    CUTE_ASSERT(expr_check(NULL, 100) == 0);

    for (tv = 0; tv < tv_nr; tv++) {
        ep = test_vector[tv].expr;
        CUTE_ASSERT(expr_check(test_vector[tv].expr, strlen(test_vector[tv].expr)) == test_vector[tv].is_valid);
        CUTE_ASSERT(ep == test_vector[tv].expr);
    }
CUTE_TEST_CASE_END

CUTE_TEST_CASE(expr_tests)
    CUTE_RUN_TEST(memory_tests);
    CUTE_RUN_TEST(expr_stack_ctx_tests);
    CUTE_RUN_TEST(expr_stack_free_tests);
    CUTE_RUN_TEST(is_expr_blank_tests);
    CUTE_RUN_TEST(expr_get_op_precedence_tests);
    CUTE_RUN_TEST(expr_get_curr_symbol_tests);
    CUTE_RUN_TEST(expr_check_tests);
    CUTE_RUN_TEST(expr_ifx2rpn_tests);
    CUTE_RUN_TEST(expr_add_tests);
    CUTE_RUN_TEST(expr_sub_tests);
    CUTE_RUN_TEST(expr_mul_tests);
    CUTE_RUN_TEST(expr_div_tests);
    CUTE_RUN_TEST(expr_eval_tests);
CUTE_TEST_CASE_END

CUTE_MAIN(expr_tests);
