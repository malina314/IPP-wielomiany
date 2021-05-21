/**
 * Author:  Mateusz Malinowski
 * Date:    March 2021
 *
 * Summary of File:
 *
 *   This file implements reading lines from stdin operations.
 */

#include "read.h"
#include "vector.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// reads all chars to '\n' or EOF and discard them
static inline void SkipLine() {
    char c;
    while ((c = (char)getchar()) != EOF && c != '\n');
}

// reads line, checks if it is comment or contains illegal characters
// sets read status
// returns pointer to buffer in which are read characters
/**
 * mamy wczytać linię, olać puste i z #
 * @param status
 * @return
 */
CVector *ReadLine(bool *isReadEnd) {
    assert(isReadEnd);

    char c = (char)getchar();

    if (feof(stdin)) {
        *isReadEnd = true;
        return NULL;
    }

    if (c == '#') {
        SkipLine();
        return NULL;
    }

    if (c == '\n') {
        return NULL;
    }

    CVector *input = CVectorNew();

     do {
         CVectorPush(input, c);
    } while (c != '\n' && (c = (char)getchar()) != EOF);

    if (c == '\n') {
        CVectorPop(input);
    }

    CVectorPush(input, '\0');

    return input;
}
