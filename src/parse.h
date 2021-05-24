/** @file
  Interfejs modułu odpowiedzialnego za konwertowanie wejścia.

  @authors Mateusz Malinowski
  @date 2021
*/

#ifndef POLYNOMIALS_PARSE_H
#define POLYNOMIALS_PARSE_H

#include "line.h"
#include "vector.h"
#include <stddef.h>

/**
 * Konwertuje wczytany wiersz na obiekt typu \ref Line reprezentujący ten
 * wiersz.
 * @param[in] str : wczytany wiersz
 * @param[in] lineNr : numer wiersza
 * @return skonwertowany wiersz
 */
Line Parse(const CVector *str, size_t lineNr);

#endif //POLYNOMIALS_PARSE_H
