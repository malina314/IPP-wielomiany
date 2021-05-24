/** @file
  Implementacja funkcji umożliwiających podstawowe operacje na wektorach.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Sprawdza, czy udało się zaalokować pamięć. Jeśli nie, kończy działanie
 * programu z kodem 1.
 * @param[in] p : wskaźnik zwrócony przez funkcję alokującą pamięć
 */
#define CHECK_PTR(p)        \
    do {                    \
        if (p == NULL) {    \
            exit(1);        \
        }                   \
    } while (0)

/**
 * początkowy rozmiar wektora jednomianów
 */
const int INITIAL_MONO_VECTOR_SIZE = 8;

/**
 * początkowy rozmiar wektora znaków
 */
const int INITIAL_CHAR_VECTOR_SIZE = 256;

CVector *CVectorNew() {
    CVector *obj = calloc(1, sizeof (CVector));
    CHECK_PTR(obj);
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
        CHECK_PTR(self->items);
        self->allocated = INITIAL_CHAR_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        CHECK_PTR(self->items);
        self->allocated *= 2;
    }

    self->items[self->size++] = c;
}

void CVectorPop(CVector *self) {
    self->size--;
}

void CVectorClear(CVector *self) {
    self->size = 0;
}

bool CVectorEmpty(CVector *self) {
    return self->size == 0;
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
        CHECK_PTR(self->items);
        self->allocated = INITIAL_MONO_VECTOR_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        CHECK_PTR(self->items);
        self->allocated *= 2;
    }

    self->items[self->size++] = m;
}
