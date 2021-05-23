/** @file
  Implementacja modułu odpowiedzialnego za konwertowanie wejścia.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "parse.h"
#include "line.h"
#include "vector.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Sprawdza czy @p pre jest prefiksem @p str.
 * @param[in] pre : łańcuch znaków
 * @param[in] str : łańcuch znaków
 * @return Czy @p pre jest prefiksem @p str?
 */
static inline bool IsPrefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

// todo ewentualnie poprawić lub usunąć
//static inline bool IsEqual(const CVector *cv, const char *str) {
//    return cv->size == strlen(str) + 1 && strcmp(cv->items, "ZERO") == 0;
//}

/**
 * Sprawdza czy @p c jest dozwolonym znakiem w wielomianie.
 * @param[in] c : znak
 * @return Czy @p c jest dozwolonym znakiem w wielomianie?
 */
static inline bool IsLegalCaracter(char c) {
    return isdigit(c) || c == '-' || c == '+' || c == '(' || c == ')' ||
        c == ',';
}

/**
 * Sprawdza czy wiersz zawiera niedozwolone dla wielomianu znaki.
 * @param[in] str : wiersz
 * @return Czy wiersz zawiera niedozwolone dla wielomianu znaki?
 */
static bool HasIllegalCharacters(const CVector *str) {
    for (size_t i = 0; i < str->size - 1; ++i) {
        if (!IsLegalCaracter(str->items[i])) {
            return true;
        }
    }
    return false;
}

/**
 * Sprawdza czy @p c jest cyfrą lub znakiem '-'.
 * @param[in] c : znak
 * @return Czy @p c jest cyfrą lub znakiem '-'?
 */
static inline bool IsDigitOrMinus(char c) {
    return isdigit(c) || c == '-';
}

/**
 * Sprawdza czy polecenie DEG_BY zawiera argument.
 * @param[in] str : wiersz
 * @return Czy polecenie DEG_BY zawiera argument?
 */
static inline bool HasDegByAnArgument(const CVector *str) {
    return str->size >= 8 && str->items[6] == ' ' && isdigit(str->items[7]);
}

/**
 * Sprawdza czy polecenie AT zawiera argument.
 * @param[in] str : wiersz
 * @return Czy polecenie AT zawiera argument?
 */
static inline bool HasAtAnArgument(const CVector *str) {
    return str->size >= 4 && str->items[2] == ' ' &&
            IsDigitOrMinus(str->items[3]);
}

/**
 * Konwertuje wiersz na obiekt typu \ref Line reprezentujący wiersz zawierający
 * polecenie.
 * @param[in] str : wiersz
 * @param[in] lineNr : numer linii
 * @return skonwertowany wiersz
 */
static Line ParseCommand(const CVector *str, size_t lineNr) {
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
                fprintf(stderr, "ERROR %zu DEG BY WRONG VARIABLE\n", lineNr);
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

/**
 * Konwertuje wykładnik jednomianu. Funkcja ustawia @p end na pierwszy znak po
 * skonwertowanym fragmencie (dla poprawnego jednomianu jest to ')').
 * Funkcja ustawia @p err na true jeżeli @p begin nie wskazuje na cyfrę lub gdy
 * wykładnik wykracza poza zakres lub gdy po wykładniku nie stoi znak ')'.
 * @param[in] begin : początek wykładnika
 * @param[out] end : wskaźnik na pierwszy znak po skonwertowanym fragmencie
 * @param[out] err : flaga błędu
 * @return wykładnik
 */
static int ParseExp(char *begin, char **end, bool *err) {
    if (!isdigit(*begin)) {
        *err = true;
        return 0;
    }

    errno = 0;
    long x = strtol(begin, end, 10);

    if (errno == ERANGE || **end != ')' || x > (long)INT_MAX) {
        *err = true;
        return 0;
    }

    return x;
}

static Poly ParsePolyHelper(char *begin, char **end, bool *err);

/**
 * Konwertuje jednomian. Funkcja ustawia @p end na pierwszy znak po
 * skonwertowanym fragmencie (dla poprawnego jednomianu jest to ')').
 * Funkcja ustawia @p err na true jeżeli @p begin nie wskazuje na '\0' lub gdy
 * jednomian jest niepoprawny.
 * @param[in] begin : początek jednomianu (pierwszy znak po '(')
 * @param[out] end : wskaźnik na pierwszy znak po skonwertowanym fragmencie
 * @param[out] err : flaga błędu
 * @return jednomian
 */
static Mono ParseMono(char *begin, char **end, bool *err) {
    if (*begin == '\0') {
        *err = true;
        return (Mono) {};
    }

    Poly p;

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
}

/**
 * Konwertuje wielomian. Poprawny wielomian zaczyna się znakiem '(', cyfrą lub
 * znakiem '-'.
 * Funkcja ustawia @p end na pierwszy znak po
 * skonwertowanym fragmencie (dla poprawnego wielomianu jest to '\0' lub ',').
 * Funkcja ustawia @p err na true jeżeli @p begin nie wskazuje na '\0' lub gdy
 * wielomian jest niepoprawny.
 * @param[in] begin : początek wielomianu
 * @param[out] end : wskaźnik na pierwszy znak po skonwertowanym fragmencie
 * @param[out] err : flaga błędu
 * @return wielomian
 */
static Poly ParsePolyHelper(char *begin, char **end, bool *err) {
    if (*begin == '\0') {
        *err = true;
        return (Poly) {};
    }

    if (IsDigitOrMinus(*begin)) { // parsowany wielomian jest współczynnikiem
        errno = 0;
        poly_coeff_t x = strtoll(begin, end, 10);
        if (errno == ERANGE || (**end != ',' && **end != '\0')) {
            *err = true;
        }
        return PolyFromCoeff(x);
    }
    else { // parsowany wielomian jest jednomianem lub sumą jednomianów
        MVector monos = MVectorNew();

        while (true) {
            if (*begin != '(') {
                *err = true;
                MVectorDeepFree(&monos);
                return (Poly) {};
            }

            Mono m = ParseMono(begin + 1, end, err);
            if (*err) {
                MVectorDeepFree(&monos);
                return (Poly) {};
            }
            MVectorPush(&monos, m);

            // po wyjściu z ParseMono *end wskazuje na ')' kończący jednomian
            (*end)++;

            if (**end == '\0' || **end == ',') { // poprawny koniec wielomianu
                break;
            }

            if (**end != '+') { // niepoprawna suma jednomianów
                *err = true;
                MVectorDeepFree(&monos);
                return (Poly) {};
            }

            begin = (*end) + 1;
        }

        Poly p = PolyAddMonos(monos.size, monos.items);

        MVectorFree(&monos);

        return p;
    }
}

/**
 * Konwertuje wiersz na obiekt typu \ref Line reprezentujący wiersz zawierający
 * wielomian.
 * @param[in] str : wiersz
 * @param[in] lineNr : numer linii
 * @return skonwertowany wiersz
 */
static Line ParsePoly(const CVector *str, size_t lineNr) {
    if (HasIllegalCharacters(str)) {
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