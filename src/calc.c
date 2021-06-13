/** @file
  Plik zawiera funkcję główną kalkulatora wykonującego działania na wielomianach
  stosującego odwrotną notację polską.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "line.h"
#include "parse.h"
#include "read.h"
#include "stack.h"
#include "vector.h"

#include <stdbool.h>
#include <stdio.h>

/// błąd oznaczający zbyt mało argumentów na stosie
#define STACK_UNDERFLOW "STACK UNDERFLOW"

/**
 * Wykonuje polecenie lub wstawia wielomian na stos.
 * @param[in] line : wiersz z poleceniem lub wielomianem
 * @param[in,out] stack : stos
 * @param[in] lineNr : numer wiersza
 */
static inline void Calc(const Line *line, Stack *stack, size_t lineNr) {
    if (line->status == POLY) {
        StackPush(stack, line->p);
    }
    else {
        switch (line->c) {
            case ZERO:
                StackPush(stack, PolyZero());
                break;
            case IS_COEFF:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyIsCoeff(&p));
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);

                }
                break;
            case IS_ZERO:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyIsZero(&p));
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case CLONE:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPush(stack, PolyClone(&p));
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case ADD:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    if (!StackEmpty(stack)) {
                        Poly q = StackTop(stack);
                        StackPop(stack);
                        StackPush(stack, PolyAdd(&p, &q));
                        PolyDestroy(&p);
                        PolyDestroy(&q);
                    }
                    else {
                        StackPush(stack, p);
                        PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                    }
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case MUL:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    if (!StackEmpty(stack)) {
                        Poly q = StackTop(stack);
                        StackPop(stack);
                        StackPush(stack, PolyMul(&p, &q));
                        PolyDestroy(&p);
                        PolyDestroy(&q);
                    }
                    else {
                        StackPush(stack, p);
                        PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                    }
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case NEG:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    StackPush(stack, PolyNeg(&p));
                    PolyDestroy(&p);
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case SUB:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    if (!StackEmpty(stack)) {
                        Poly q = StackTop(stack);
                        StackPop(stack);
                        StackPush(stack, PolySub(&p, &q));
                        PolyDestroy(&p);
                        PolyDestroy(&q);
                    }
                    else {
                        StackPush(stack, p);
                        PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                    }
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case IS_EQ:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    if (!StackEmpty(stack)) {
                        Poly q = StackTop(stack);
                        printf("%d\n", PolyIsEq(&p, &q));
                    }
                    else {
                        PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                    }
                    StackPush(stack, p);
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case DEG:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyDeg(&p));
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case DEG_BY:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyDegBy(&p, (size_t)line->arg));
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case AT:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    StackPush(stack, PolyAt(&p, line->arg));
                    PolyDestroy(&p);
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case PRINT:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    PolyPrint(&p, true);
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
            case POP:
                if (!StackEmpty(stack)) {
                    StackHardPop(stack);
                }
                else {
                    PrintErrorMsg(lineNr, STACK_UNDERFLOW);
                }
                break;
        }
    }
}

/**
 * Funkcja główna programu, realizuje zadanie kalkulatora przetwarzając kolejne
 * wiersze wejścia.
 * @return 0
 */
int main() {
    Stack stack = StackNew();
    CVector *input = CVectorNew();
    size_t lineNr = 1;
    bool isReadEnd = false;

    while (!isReadEnd) {
        isReadEnd = ReadLine(input);
        if (!CVectorEmpty(input)) {
            Line line = Parse(input, lineNr);

            CVectorClear(input);

            if (IsCorrectLine(&line)) {
                Calc(&line, &stack, lineNr);
            }
        }
        lineNr++;
    }

    StackFree(&stack);
    CVectorFree(input);

    return 0;
}
