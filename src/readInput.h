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

//typedef enum {
//    READ_OK,
//    WRONG_COMMAND,
//    WRONG_POLY,
//    READ_END,
//    READ_COMMENT,
//    READ_EMPTY_LINE
//} ReadStatus;
//
CVector *ReadLine(bool *isReadEnd);

#endif //SIMILAR_LINES_READINPUT_H

//todo jak to ogarnę to zmienić nazwę na coś innego
