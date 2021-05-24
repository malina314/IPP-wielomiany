/** @file
  Plik udostępnia typ wyliczeniowy reprezentujący polecenie oraz strukturę
  przechowującą wiersz wraz z funkcjami umożliwiającymi podstawowe operacje na
  tej strukturze.

  @authors Mateusz Malinowski
  @date 2021
*/

#ifndef POLYNOMIALS_LINE_H
#define POLYNOMIALS_LINE_H

#include "poly.h"
#include <stdbool.h>

/**
 * To jest typ wyliczeniowy reprezentujący polecenie.
 */
typedef enum {
    ZERO, IS_COEFF, IS_ZERO, CLONE, ADD, MUL, NEG, SUB, IS_EQ, DEG, DEG_BY, AT,
    PRINT, POP
} Command;

/**
 * To jest typ wyliczeniowy służący do określania statusu wiersza.
 */
typedef enum {
    POLY, COMMAND, ERROR
} LineStatus;

/**
 * To jest struktura przechowująca wiersz zawierający polecenie lub wielomian.
 */
typedef struct {
    /**
     * To jest unia przechowująca wielomian lub strukutrę złożoną z polecenia
     * i argumentu. Jeżeli polecenie nie ma argumetu to pole `arg` nie jest
     * używane.
     */
    union {
        Poly p; ///< wielomian
        struct {
            Command c; ///< polecenie
            poly_coeff_t arg; ///< argument polecenia DEG_BY lub AT
        };
    };
    LineStatus status; ///< status wiersza
} Line;

/**
 * Zwraca niepoprawny wiersz reprezentujący sytuację kiedy na wejściu użytkownik
 * wprowadził nieprawidłową komendę lub wielomian.
 * @return niepoprawny wiersz
 */
Line WrongLine();

/**
 * Sprawdza czy wiersz jest poprawny.
 * @param[in] line : wiersz
 * @return : Czy wiersz jest poprawny?
 */
bool IsCorrectLine(const Line *line);

/**
 * Tworzy obiekt typu \ref Line reprezentujący wiersz zawierający polecenie
 * @p command.
 * @param[in] command : polecenie
 * @return wiersz zawierający polecenie
 */
Line CommandLine(Command command);

/**
 * Tworzy obiekt typu \ref Line reprezentujący wiersz zawierający polecenie
 * @p command z argumentem @p arg.
 * @param[in] command : polecenie
 * @param[in] arg : argument
 * @return wiersz zawierający polecenie z argumentem
 */
Line CommandLineWithArg(Command command, poly_coeff_t arg);

/**
 * Tworzy obiekt typu \ref Line reprezentujący wiersz zawierający wielomian @p p.
 * @param[in] p : wielomian
 * @return wiersz zawierający wielomian
 */
Line PolyLine(Poly p);

#endif //POLYNOMIALS_LINE_H
