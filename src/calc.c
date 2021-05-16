#include "readInput.h"
#include "line.h"
#include "stack.h"
#include "vector.h"
#include "parse.h"

#include <stdio.h>
#include <stdbool.h>

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

void Calc(Line *line, Stack *stack, size_t lineNr) {
    if (line->is_poly) {
        StackPush(stack, *line->p);
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
                    printf("%d\n", PolyDegBy(&p, (size_t)line->arg));
                }
                else {
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
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
                    fprintf(stderr, "ERROR %zu STACK UNDERFLOW\n", lineNr);
                }
                break;
            case PRINT:
                if (!StackEmpty(stack)) {
                    Poly p = StackTop(stack);
                    PolyPrint(&p, true);
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

//char *zrobCosZOutputemReadLineaDawniejGetLinea();
void testy();


int main() {

    //testy();

//    char *end;
//    errno = 0;
//    unsigned long long x = strtoull("18446744073709551616", &end, 10);
//    if (errno == ERANGE) {
//        printf("wczytal sie %llu", x);
//    } else {
//        printf("poprawnie sie nie wczytal");
//    }
//    printf("%lu, %c", x, *end);
// =======================================================================

    Stack stack = StackNew();
    size_t lineNr = 1;
    bool isReadEnd = false;

    while (!isReadEnd) {
        CVector *input = ReadLine(&isReadEnd);
        if (input != NULL) {
            Line line = Parse(input, lineNr);
            if (IsCorrectLine(&line)) {
                Calc(&line, &stack, lineNr);
            }
            LineFree(&line);
            CVectorFree(input);
        }
        lineNr++;
    }

    StackFree(&stack);

    return 0;
}

//==================================================================
//==================================================================
//==================================================================
//==================================================================
//==================================================================
//==================================================================


#include <stdarg.h>

#define CHECK_PTR(p)        \
    do {                    \
        if (p == NULL) {    \
            exit(1);        \
        }                   \
    } while (0)

#define C PolyFromCoeff

static Mono M(Poly p, poly_exp_t n) {
    return MonoFromPoly(&p, n);
}

static Poly MakePolyHelper(poly_exp_t dummy, ...) {
    va_list list;
    va_start(list, dummy);
    size_t count = 0;
    while (true) {
        va_arg(list, Poly);
        if (va_arg(list, poly_exp_t) < 0)
            break;
        count++;
    }
    va_start(list, dummy);
    Mono *arr = calloc(count, sizeof(Mono));
    CHECK_PTR(arr);
    for (size_t i = 0; i < count; ++i) {
        Poly p = va_arg(list, Poly);
        arr[i] = MonoFromPoly(&p, va_arg(list, poly_exp_t));
        assert(i == 0 || MonoGetExp(&arr[i]) > MonoGetExp(&arr[i - 1]));
    }
    va_end(list);
    Poly res = PolyAddMonos(count, arr);
    free(arr);
    return res;
}

#define P(...) MakePolyHelper(0, __VA_ARGS__, PolyZero(), -1)

void testy() {
    Poly p;

    p = C(0);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = C(1);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(C(1), 1);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(C(1), 0);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(C(0), 10);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(C(1), 1, C(2), 2, C(3), 3);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(P(C(1), 2), 3);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(P(P(P(C(2), 3), 4, P(C(7), 8), 9), 5), 6);
    PolyPrint(&p, true);
    PolyDestroy(&p);

    p = P(P(P(P(C(2), 3), 4, P(C(7), 8), 9), 5), 6, C(10), 11);
    PolyPrint(&p, true);
    PolyDestroy(&p);
}