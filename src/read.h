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
 * Ustawia @p isReadEnd na true, jeśli dotrze do EOF.
 * @param[out] isReadEnd : flaga czy to koniec wejścia
 * @return NULL gdy wiersz jest pusty lub zaczyna się znakiem '#', w przeciwnym
 * razie wczytany wiersz
 */
CVector *ReadLine(bool *isReadEnd);

#endif //SIMILAR_LINES_READINPUT_H
