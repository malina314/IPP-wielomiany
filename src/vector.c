/**
 * Author:  Mateusz Malinowski
 * Date:    March 2021
 *
 * Summary of File:
 *
 *   This file implements new, free and push functions for each vector type
 *   defined in related header.
 */

#include "vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int INITIAL_VECTOR_SIZE = 4;
const int INITIAL_CHAR_VECTOR_SIZE = 32;

CVector CVectorNew() {
    CVector obj = {NULL, 0, 0};
    return obj;
}

void CVectorFree(CVector *self) {
    free(self->items);
}

void CVectorPush(CVector *self, char c) {
    size_t typeSize = sizeof c;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_CHAR_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_CHAR_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = c;
}

ULLVector ULLVectorNew() {
    ULLVector obj = {NULL, 0, 0};
    return obj;
}

void ULLVectorFree(ULLVector *self) {
    free(self->items);
}

void ULLVectorPush(ULLVector *self, unsigned long long x) {
    size_t typeSize = sizeof x;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = x;
}


LLVector LLVectorNew() {
    LLVector obj = {NULL, 0, 0};
    return obj;
}

void LLVectorFree(LLVector *self) {
    free(self->items);
}

void LLVectorPush(LLVector *self, long long x) {
    size_t typeSize = sizeof x;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = x;
}


DVector DVectorNew() {
    DVector obj = {NULL, 0, 0};
    return obj;
}

void DVectorFree(DVector *self) {
    free(self->items);
}

void DVectorPush(DVector *self, double x) {
    size_t typeSize = sizeof x;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = x;
}


SVector SVectorNew() {
    SVector obj = {NULL, 0, 0};
    return obj;
}

void SVectorFree(SVector *self) {
    for (size_t i = 0; i < self->size; ++i) {
        free(self->items[i]);
    }
    free(self->items);
}

void SVectorPush(SVector *self, char *str) {
    size_t typeSize = sizeof str;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size] = malloc(strlen(str) + 1);
    strcpy(self->items[self->size++], str);
}


ULLVectorVector ULLVectorVectorNew() {
    ULLVectorVector obj = {NULL, 0, 0};
    return obj;
}

void ULLVectorVectorFree(ULLVectorVector *self) {
    for (size_t i = 0; i < self->size; ++i) {
        ULLVectorFree(&self->items[i]);
    }
    free(self->items);
}

void ULLVectorVectorPush(ULLVectorVector *self, ULLVector v) {
    size_t typeSize = sizeof v;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = v;
}