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
void StackFree(Stack *self);
void StackPush(Stack *self, Poly p);
void StackPop(Stack *self);
void StackHardPop(Stack *self);
Poly StackTop(Stack *self);
bool StackEmpty(Stack *self);

#endif //POLYNOMIALS_STACK_H
