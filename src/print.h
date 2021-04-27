#ifndef POLYNOMIALS_PRINT_H
#define POLYNOMIALS_PRINT_H

#include "poly.h"
#include <stdio.h>

static void PolyPrint(Poly *p, int lvl);

static void MonoPrint(Mono *m, int lvl) {
    PolyPrint(&m->p, lvl + 1);
    setbuf(stdout, 0);
    printf("x_%d^%d", lvl, m->exp);
}

static void PolyPrint(Poly *p, int lvl) {
    if (PolyIsCoeff(p)) {
        setbuf(stdout, 0);
        printf("%ld*", p->coeff);
    }
    else {
        MonoPrint(&p->arr[0], lvl);
        for (size_t i = 1; i < p->size; ++i) {
            setbuf(stdout, 0);
            printf("+");
            MonoPrint(&p->arr[i], lvl);
        }
    }
    if (lvl == 0) {
        setbuf(stdout, 0);
        printf("\n");
    }
}

#endif //POLYNOMIALS_PRINT_H
