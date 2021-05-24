/** @file
  Implementacja modułu odpowiedzialnego za wczytywanie wejścia.

  @authors Mateusz Malinowski
  @date 2021
*/

#include "read.h"
#include "vector.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * Wczytuje znaki aż do napotkania '\n' lub EOF i porzuca je.
 */
static inline void SkipLine() {
    char c;
    while ((c = (char)getchar()) != EOF && c != '\n');
}

bool ReadLine(CVector *input) {
    assert(input && CVectorEmpty(input));

    char c = (char)getchar();

    if (feof(stdin)) {
        return true;
    }

    if (c == '#') {
        SkipLine();
        return false;
    }

    if (c == '\n') {
        return false;
    }

    do {
         CVectorPush(input, c);
    } while (c != '\n' && (c = (char)getchar()) != EOF);

    if (c == '\n') {
        CVectorPop(input);
    }

    CVectorPush(input, '\0');

    return false;
}
