/**
 * Author:  Mateusz Malinowski
 * Date:    March 2021
 *
 * Summary of File:
 *
 *   This header provides vector structure of following types:
 *    -char
 *    -unsigned long long
 *    -long long
 *    -double
 *    -string
 *    -unsigned long long vector
 *   and foreach following operations
 *    -new
 *    -push
 *    -free
 */

#ifndef SIMILAR_LINES_VECTOR_H
#define SIMILAR_LINES_VECTOR_H

#include <stdlib.h>

typedef struct {
    char *items;
    size_t size;
    size_t allocated;
} CVector;

typedef struct {
    unsigned long long *items;
    size_t size;
    size_t allocated;
} ULLVector;

typedef struct {
    long long *items;
    size_t size;
    size_t allocated;
} LLVector;

typedef struct {
    double *items;
    size_t size;
    size_t allocated;
} DVector;

typedef struct {
    char **items;       // strings have to end with '\0'
    size_t size;
    size_t allocated;
} SVector;

typedef struct {
    ULLVector *items;
    size_t size;
    size_t allocated;
} ULLVectorVector;

CVector *CVectorNew();
void CVectorFree(CVector *self);
void CVectorPush(CVector *self, char c);
void CVectorPop(CVector *self);

ULLVector ULLVectorNew();
void ULLVectorFree(ULLVector *self);
void ULLVectorPush(ULLVector *self, unsigned long long x);

LLVector LLVectorNew();
void LLVectorFree(LLVector *self);
void LLVectorPush(LLVector *self, long long x);

DVector DVectorNew();
void DVectorFree(DVector *self);
void DVectorPush(DVector *self, double x);

SVector SVectorNew();
void SVectorFree(SVector *self);
void SVectorPush(SVector *self, char *str);

ULLVectorVector ULLVectorVectorNew();
void ULLVectorVectorFree(ULLVectorVector *self);
void ULLVectorVectorPush(ULLVectorVector *self, ULLVector v);

#endif //SIMILAR_LINES_VECTOR_H
