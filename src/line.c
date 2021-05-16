#include "line.h"
#include "poly.h"
#include <stdbool.h>

Line WrongLine() {
    return (Line) {.p = NULL, .is_poly = true};
}

bool IsCorrectLine(Line *line) {
    return line->p != NULL || line->is_poly == false;
}

Line CommandLine(Command command) {
    return (Line) {.c = command, .is_poly = false};
}

Line CommandLineWithArg(Command command, poly_coeff_t arg) {
    return (Line) {.c = command, .arg = arg, .is_poly = false};
}

Line PolyLine(Poly *p) {
    return (Line) {.p = p, .is_poly = true};
}

void LineFree(Line *self) {
    if (self->is_poly) {
        PolyDestroy(self->p);
    }
}