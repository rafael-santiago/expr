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
#include <string.h>

CUTE_TEST_CASE(memory_tests)
    char *data = NULL;
    data = expr_alloc(10);
    CUTE_ASSERT(data != NULL);
    // INFO(Rafael): No SIGSEGV must happen. If "data" was not properly freed, the memory leak check
    //               system from unit test library will detect this issue.
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

CUTE_TEST_CASE(expr_tests)
    CUTE_RUN_TEST(memory_tests);
    CUTE_RUN_TEST(expr_stack_ctx_tests);
    CUTE_RUN_TEST(expr_stack_free_tests);
CUTE_TEST_CASE_END

CUTE_MAIN(expr_tests);
