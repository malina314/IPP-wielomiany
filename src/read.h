/** @file
  Interfejs modułu odpowiedzialnego za wczytywanie wejścia.

  @authors Mateusz Malinowski
  @date 2021
*/

#ifndef SIMILAR_LINES_READINPUT_H
#define SIMILAR_LINES_READINPUT_H

#include "vector.h"
#include <stdbool.h>

/**
 * Wczytuje wiersz znak po znaku.
 * @param[out] input : pusty bufor na wczytywane znaki
 * @return `true` gdy natrafi na EOF, w przeciwnym razie `false`
 */
bool ReadLine(CVector *input);

#endif //SIMILAR_LINES_READINPUT_H
