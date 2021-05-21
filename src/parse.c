#include "line.h"
#include "parse.h"
#include "vector.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * Sprawdza czy @pre jest prefiksem @str.
 * @param[in] pre : łańcuch znaków
 * @param[in] str : łańcuch znaków
 * @return Czy @pre jest prefiksem @str?
 */
static inline bool IsPrefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

// todo coś tu nie działa
//static inline bool IsEqual(const CVector *cv, const char *str) {
//    return cv->size == strlen(str) + 1 && strcmp(cv->items, "ZERO") == 0;
//}

/**
 *
 * @param c
 * @return
 */
static inline bool IsLegalCaracter(char c) {
    return isupper(c) || isdigit(c) || c == '-' || c == '+' || c == '(' ||
        c == ')' || c == ' ' || c == '_' || c == ',' || c == '\n';
}

static bool IsCorrectString(const CVector *str) {
    for (size_t i = 0; i < str->size - 1; ++i) {
        if (!IsLegalCaracter(str->items[i])) {
            return false;
        }
    }
    return true;
}

static inline bool IsDigitOrSign(char c) {
    return isdigit(c) || c == '-';
}

static inline bool HasDegByAnArgument(const CVector *str) {
    return str->size >= 8 && str->items[6] == ' ' && isdigit(str->items[7]);
}

static inline bool HasAtAnArgument(const CVector *str) {
    return str->size >= 4 && str->items[2] == ' ' &&
        IsDigitOrSign(str->items[3]);
}

Line ParseCommand(const CVector *str, size_t lineNr) {
//    if (!IsCorrectString(str)) {
//        fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lineNr);
//        return WrongLine();
//    }

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
    if (IsPrefix("DEG_BY", str->items)) {
        if (HasDegByAnArgument(str)) {
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
    if (IsPrefix("AT", str->items)) {
        if (HasAtAnArgument(str)) {
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

    fprintf(stderr, "ERROR %zu WRONG COMMAND\n", lineNr);
    return WrongLine();
}

// begin wskazuje na cyfrę bez znaku
// end po wyjściu wskazuje na )
int ParseExp(char *begin, char **end, bool *err) {
    if (!isdigit(*begin)) {
        *err = true;
        return 0;
    }

    errno = 0;
    long x = strtol(begin, end, 10);

    if (errno == ERANGE || **end != ')' || x > (long)INT_MAX) {
        *err = true;
    }

    return x;
}

Poly ParsePolyHelper(char *begin, char **end, bool *err);

// begin wskazuje na pierwszy digit (lub znak +-) lub nawias kolejnego poly
// end po wyjściu wskazuje na )
Mono ParseMono(char *begin, char **end, bool *err) {
    if (*begin == '\0') {
        *err = true;
        return (Mono) {};
    }

    Poly p;

    // można bez ifa bo w sumie wielomian i tak sprawdza czy jest współczynnikiem czy jednomianem

    //if (*begin == '(') {
        p = ParsePolyHelper(begin, end, err);
        if (*err) {
            return (Mono) {};
        }

        if (**end == '\0') {
            *err = true;
            PolyDestroy(&p);
            return (Mono) {};
        }

        // teraz *end wskazuje na ','
        int exp = ParseExp((*end) + 1, end, err);

        if (*err) {
            PolyDestroy(&p);
            return (Mono) {};
        }

        return MonoFromPoly(&p, exp);
//    }
//    else { // begin wskazuje na pierwszy digit (lub znak +-)
//        //
//    }
}

// begin wskazuje na ( lub digit ze znakiem -> tylko współczynnik
// end wskazuje na \0 lub ,
Poly ParsePolyHelper(char *begin, char **end, bool *err) {
    if (*begin == '\0') {
        *err = true;
        return (Poly) {};
    }

    if (IsDigitOrSign(*begin)) { // parsowany wielomian jest współczynnikiem
        errno = 0;
        poly_coeff_t x = strtoll(begin, end, 10);
        if (errno == ERANGE || (**end != ',' && **end != '\0')) {
            *err = true;
        }
        return PolyFromCoeff(x);
    }
    else { // parsowany wielomian jest jednomianem lub sumą jednomianów
        MVector monos = MVectorNew();

        // niezmiennik: begin wskazuje na (
        while (true) {
            Mono m = ParseMono(begin + 1, end, err);
            if (*err) {
                MVectorDeepFree(&monos);
                return (Poly) {};
            }
            MVectorPush(&monos, m);

            // po wyjściu z ParseMono *end wskazuje na ) kończący jednomian
            // wielomian ma postać (..)\0 lub (..)+(..)+...+(..),
            // ale nigdy ((..)+(..)+...+(..))

            if (*(*end + 1) == '\0' || *(*end + 1) == ',') { // todo po testach wyciągnąć do osobnej funkcji
                (*end)++;
                break;
            }

            if (*(*end + 1) != '+' || *(*end + 2) != '(') {
                *err = true;
                MVectorDeepFree(&monos);
                return (Poly) {};
            }

            begin = (*end) + 2;
        }


        Poly p = PolyAddMonos(monos.size, monos.items);

        MVectorFree(&monos);

        return p;
    }
}

// (((((2,3),4)+((7,8),9)),5),6)+(10,11)
Line ParsePoly(const CVector *str, size_t lineNr) {
    if (!IsCorrectString(str)) {
        fprintf(stderr, "ERROR %zu WRONG POLY\n", lineNr);
        return WrongLine();
    }

    bool err = false;
    char *end;

    Poly *p = malloc(sizeof (Poly));
    *p = ParsePolyHelper(str->items, &end, &err);

    if (err || *end != '\0') {
        fprintf(stderr, "ERROR %zu WRONG POLY\n", lineNr);
        free(p);
        return WrongLine();
    }

    return PolyLine(p);
}

Line Parse(const CVector *str, size_t lineNr) {
    if (isalpha(str->items[0])) {
        return ParseCommand(str, lineNr);
    }
    else {
        return ParsePoly(str, lineNr);
    }
}