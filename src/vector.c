/** @file
  Implementacja funkcji umożliwiających podstawowe operacje na wektorach.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "vector.h"
#include <stdlib.h>

/**
 * początkowy rozmiar wektora jednomianów
 */
const int INITIAL_MONO_VECTOR_SIZE = 8;

/**
 * początkowy rozmiar wektora znaków
 */
const int INITIAL_CHAR_VECTOR_SIZE = 32;

CVector *CVectorNew() {
    CVector *obj = calloc(1, sizeof (CVector));
    return obj;
}

void CVectorFree(CVector *self) {
    free(self->items);
    free(self);
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

void CVectorPop(CVector *self) {
    self->size--;
}

MVector MVectorNew() {
    return (MVector) {NULL, 0, 0};
}

void MVectorFree(MVector *self) {
    free(self->items);
}

void MVectorDeepFree(MVector *self) {
    for (size_t i = 0; i < self->size; ++i) {
        MonoDestroy(&self->items[i]);
    }
    free(self->items);
}

void MVectorPush(MVector *self, Mono m) {
    size_t typeSize = sizeof m;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_MONO_VECTOR_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_MONO_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = m;
}
