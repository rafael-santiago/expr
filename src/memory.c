/*
 *                                Copyright (C) 2017 by Rafael Santiago
 *
 * This is a free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "memory.h"
#include <stdio.h>

void *expr_alloc(const size_t segsize) {
    void *maddr = malloc(segsize);

    if (maddr == NULL) {
        printf("ERROR: no memory! Exiting...\n");
        exit(1);
    }

    return maddr;
}

void expr_free(void *maddr) {
    if (maddr != NULL) {
        free(maddr);
    }
}
