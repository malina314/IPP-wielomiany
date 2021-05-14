#include "readInput.h"
#include "line.h"
#include "stack.h"
#include "vector.h"

#include <stdio.h>

//ZERO – wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru;
//IS_COEFF – sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem – wypisuje na standardowe wyjście 0 lub 1;
//IS_ZERO – sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru – wypisuje na standardowe wyjście 0 lub 1;
//CLONE – wstawia na stos kopię wielomianu z wierzchołka;
//ADD – dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę;
//MUL – mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn;
//NEG – neguje wielomian na wierzchołku stosu;
//SUB – odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę;
//IS_EQ – sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje na standardowe wyjście 0 lub 1;
//DEG – wypisuje na standardowe wyjście stopień wielomianu (−1 dla wielomianu tożsamościowo równego zeru);
//DEG_BY idx – wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze idx (−1 dla wielomianu tożsamościowo równego zeru);
//AT x – wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji;
//PRINT – wypisuje na standardowe wyjście wielomian z wierzchołka stosu;
//POP – usuwa wielomian z wierzchołka stosu.

void Calc(Line line, Stack *stack, size_t lineNr) {
    if (line.is_poly) {
        StackPush(stack, line.p);
    }
    else {
        switch (line.c) {
            case ZERO:
                StackPush(stack, PolyZero());
                break;
            case IS_COEFF:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyIsCoeff(&p));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case IS_ZERO:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyIsZero(&p));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case CLONE:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPush(stack, PolyClone(&p));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                    }
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                    }
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                    }
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                        fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                    }
                    StackPush(stack, p);
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case DEG:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyDeg(&p));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case DEG_BY:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    printf("%d\n", PolyDegBy(&p, (size_t)line.arg));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case AT:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    StackPop(stack);
                    StackPush(stack, PolyAt(&p, line.arg));
                    PolyDestroy(&p);
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case PRINT:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    PolyPrint(&p);
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case POP:
                StackHardPop(stack);
                break;
        }
    }
}

char *zrobCosZOutputemReadLineaDawniejGetLinea();

int main() {
    Stack stack = StackNew();
    size_t lineNr = 0;
    while (IsNextLine()) {
        char *input = zrobCosZOutputemReadLineaDawniejGetLinea();
        // wcześniej ReadLine parsował

        Line line = parseLine(input);
        if (line != NULL) {
            lineNr++;
            Calc(line);
        }
    }
    return 0;
}

char *zrobCosZOutputemReadLineaDawniejGetLinea() {
    ReadStatus status;
    char *input = ReadLine(&status);

    switch (status) {
        case READ_OK:
            return input;
        case WRONG_COMMAND:
            fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lineNr);
            break;
        case WRONG_POLY:
            fprintf(stderr, "ERROR %zu WRONG POLY\n", lineNr);
            break;
        case READ_END:
            exit(0); //todo coś lepszego
        case READ_COMMENT:
        case READ_EMPTY_LINE:
            return zrobCosZOutputemReadLineaDawniejGetLinea();
    }

}