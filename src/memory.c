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
