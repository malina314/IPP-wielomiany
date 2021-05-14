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

typedef enum {
    READ_OK,
    WRONG_COMMAND,
    WRONG_POLY,
    READ_END,
    READ_COMMENT,
    READ_EMPTY_LINE
} ReadStatus;

char *ReadLine(ReadStatus *status);

#endif //SIMILAR_LINES_READINPUT_H

//todo jak to ogarnę to zmienić nazwę na coś innego
