#ifndef POLYNOMIALS_LINE_H
#define POLYNOMIALS_LINE_H

#include "poly.h"
#include <stdbool.h>

typedef enum {
    ZERO, IS_COEFF, IS_ZERO, CLONE, ADD, MUL, NEG, SUB, IS_EQ, DEG, DEG_BY, AT,
    PRINT, POP
} Command;

typedef struct {
    union {
        Poly p;
        struct {
            Command c;
            poly_coeff_t arg; // castowanie z signed na unsigbed jest ok
        };
    };
    bool is_poly;
} Line;

#endif //POLYNOMIALS_LINE_H
