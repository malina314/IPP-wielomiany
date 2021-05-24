/** @file
  Implementacja funkcji umożliwiających podstawowe operacje na wierszu.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "line.h"
#include "poly.h"
#include <stdbool.h>

Line WrongLine() {
    return (Line) {.status = ERROR};
}

bool IsCorrectLine(const Line *line) {
    return line->status != ERROR;
}

Line CommandLine(Command command) {
    return (Line) {.c = command, .status = COMMAND};
}

Line CommandLineWithArg(Command command, poly_coeff_t arg) {
    return (Line) {.c = command, .arg = arg, .status = COMMAND};
}

Line PolyLine(Poly p) {
    return (Line) {.p = p, .status = POLY};
}
