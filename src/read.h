/** @file
  Interfejs modułu odpowiedzialnego za wczytywanie wejścia.

  @authors Mateusz Malinowski
  @date 2021
*/

#ifndef SIMILAR_LINES_READINPUT_H
#define SIMILAR_LINES_READINPUT_H

#include "vector.h"
#include <stdbool.h>

CVector *ReadLine(bool *isReadEnd);

#endif //SIMILAR_LINES_READINPUT_H
