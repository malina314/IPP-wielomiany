/** @file
  Implementacja funkcji umożliwiających podstawowe operacje na wektorach.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "stack.h"
#include "poly.h"
#include <stdbool.h>
#include <stdlib.h>

const int INITIAL_STACK_SIZE = 16;

Stack StackNew() {
    Stack obj = {NULL, 0, 0};
    return obj;
}

void StackFree(Stack *self) {
    for (size_t i = 0; i < self->size; ++i) {
        PolyDestroy(&self->items[i]);
    }
    free(self->items);
}

void StackPush(Stack *self, Poly p) {
    size_t typeSize = sizeof p;
    if (self->allocated == 0) {
        self->items = malloc(INITIAL_STACK_SIZE * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated = INITIAL_STACK_SIZE;
    } else if (self->size == self->allocated) {
        self->items = realloc(self->items, self->allocated * 2 * typeSize);
        if (self->items == NULL) {
            exit(1);
        }
        self->allocated *= 2;
    }

    self->items[self->size++] = p;
}

void StackPop(Stack *self) {
    self->size--;
}

void StackHardPop(Stack *self) {
    PolyDestroy(&self->items[--self->size]);
}

Poly StackTop(const Stack *self) {
    return self->items[self->size - 1];
}

bool StackEmpty(const Stack *self) {
    return self->size == 0;
}
