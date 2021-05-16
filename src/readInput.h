/**
 * Author:  Mateusz Malinowski
 * Date:    March 2021
 *
 * Summary of File:
 *
 *   This header provides function which reads all lines of input, converts
 *   and pushes them into LineVector.
 */

#ifndef SIMILAR_LINES_READINPUT_H
#define SIMILAR_LINES_READINPUT_H

#include "vector.h"
#include <stdbool.h>

CVector *ReadLine(bool *isReadEnd);

#endif //SIMILAR_LINES_READINPUT_H
