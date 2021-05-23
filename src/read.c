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
