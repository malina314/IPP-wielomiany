#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include "poly.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    Poly *items;
    size_t size;
    size_t allocated;
} Stack;

Stack StackNew();

void StackFree(const Stack *self);

void StackPush(const Stack *self, Poly p);

void StackPop(const Stack *self);

void StackHardPop(const Stack *self);

Poly StackTop(const Stack *self);

bool StackEmpty(const Stack *self);

#endif //POLYNOMIALS_STACK_H
