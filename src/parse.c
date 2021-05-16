#include "parse.h"
#include "line.h"
#include "vector.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

static inline bool IsPrefix(const char *pre, const char *str) { // todo chyba do wywalenia
    return strncmp(pre, str, strlen(pre)) == 0;
}

static inline bool IsLegalCaracter(char c) {
    return isupper(c) || isdigit(c) || c == '-' || c == '+' || c == '(' ||
        c == ')' || c == ' ' || c == '\n';
}

static bool IsCorrectString(const CVector *str) {
    for (size_t i = 0; i < str->size; ++i) {
        if (!IsLegalCaracter(str->items[i])) {
            return false;
        }
    }
    return true;
}

static inline bool WarunekDegBy(const CVector *str) { // todo nazwac to
    return str->size >= 8 && str->items[6] == ' ' && isdigit(str->items[7]);
}

static inline bool WarunekAt(const CVector *str) { // todo nazwac to
    return str->size >= 4 && str->items[2] == ' ' && isdigit(str->items[3]);
}

Line Parse(const CVector *str, size_t lineNr) {
    if (isalpha(str->items[0])) { // komenda
        if (!IsCorrectString(str)) {
            fprintf(stderr, "ERROR %zu COMMAND\n", lineNr);
            return WrongLine();
        }

        if (strcmp(str->items, "ZERO") == 0) {
            return CommandLine(ZERO);
        }
        if (strcmp(str->items, "IS_COEFF") == 0) {
            return CommandLine(IS_COEFF);
        }
        if (strcmp(str->items, "IS_ZERO") == 0) {
            return CommandLine(IS_ZERO);
        }
        if (strcmp(str->items, "CLONE") == 0) {
            return CommandLine(CLONE);
        }
        if (strcmp(str->items, "ADD") == 0) {
            return CommandLine(ADD);
        }
        if (strcmp(str->items, "MUL") == 0) {
            return CommandLine(MUL);
        }
        if (strcmp(str->items, "NEG") == 0) {
            return CommandLine(NEG);
        }
        if (strcmp(str->items, "SUB") == 0) {
            return CommandLine(SUB);
        }
        if (strcmp(str->items, "IS_EQ") == 0) {
            return CommandLine(IS_EQ);
        }
        if (strcmp(str->items, "DEG") == 0) {
            return CommandLine(DEG);
        }
        if (strcmp(str->items, "PRINT") == 0) {
            return CommandLine(PRINT);
        }
        if (strcmp(str->items, "POP") == 0) {
            return CommandLine(POP);
        }
        if (strcmp(str->items, "DEG_BY") == 0) {
            if (WarunekDegBy(str)) {
                char *end;
                errno = 0;
                size_t arg = strtoull(str->items + 7, &end, 10);

                if (errno == ERANGE || *end != '\0') {
                    fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n",
                            lineNr);
                    return WrongLine();
                }

                return CommandLineWithArg(DEG_BY, arg);
            }
            else {
                fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", lineNr);
                return WrongLine();
            }
        }
        if (strcmp(str->items, "AT") == 0) {
            if (WarunekAt(str)) {
                char *end;
                errno = 0;
                poly_coeff_t arg = strtoll(str->items + 3, &end, 10);

                if (errno == ERANGE || *end != '\0') {
                    fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", lineNr);
                    return WrongLine();
                }

                return CommandLineWithArg(AT, arg);
            }
            else {
                fprintf(stderr, "ERROR %zu AT WRONG VALUE\n", lineNr);
                return WrongLine();
            }
        }

        fprintf(stderr, "ERROR %zu COMMAND\n", lineNr);
        return WrongLine();
    }
    else { // wielomian
        if (!IsCorrectString(str)) { // todo chyba jest dziwny bug że zawsze jest true
            fprintf(stderr, "ERROR %zu POLY\n", lineNr);
            return WrongLine();
        }
        // todo parsowanie wielomianow
    }
}