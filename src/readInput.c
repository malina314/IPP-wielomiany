/**
 * Author:  Mateusz Malinowski
 * Date:    March 2021
 *
 * Summary of File:
 *
 *   This file implements reading lines from stdin operations.
 */

#include "readInput.h"
#include "vector.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

// reads all chars to '\n' or EOF and discard them
static inline void SkipLine() {
    char c;
    while ((c = (char)getchar()) != EOF && c != '\n');
}

bool IsLegalCaracter(char c) {
    return isalnum(c) || c == '-' || c == '+' || c == '(' || c == ')' ||
        c == ' ' || c == '\n';
}

// reads line, checks if it is comment or contains illegal characters
// sets read status
// returns pointer to buffer in which are read characters
/**
 * mamy wczytać linię, olać puste i z #
 * @param status
 * @return
 */
char *ReadLine(ReadStatus *status) {
    char c = (char)getchar();
    ReadStatus possibleError;

    if (feof(stdin)) {
        *status = READ_END;
        return NULL;
    }

    if (c == '#') {
        SkipLine();
        *status = READ_COMMENT;
        return NULL;
    }
    else if (isalpha(c)) {
        possibleError = WRONG_COMMAND;
    }
    else {
        possibleError = WRONG_POLY;
    }

    CVector input = CVectorNew();

     do {
        if (IsLegalCaracter(c)) {
            CVectorPush(&input, (char)toupper(c)); // todo wywalić to upper jeżeli nie potrzebne
        }
        else {
            *status = possibleError;
            CVectorFree(&input);
            SkipLine();
            return NULL;
        }
    } while (c != '\n' && (c = (char)getchar()) != EOF);

    if (c != '\n') {
        CVectorPush(&input, '\n');
    }

    CVectorPush(&input, '\0');

    // jeżeli linia zawierała tylko '\n'
    if (input.size == 2) {
        *status = READ_EMPTY_LINE;
        CVectorFree(&input);
        return NULL;
    }

    *status = READ_OK;

    return input.items;
}



// read input line by line, converts them to proper object and pushes them into
// line vector.
/*void readInput(LineVector *lv) {
    int nr = 0;

    while (1) {
        Line *line = LineNew(++nr);
        int status = ReadLine(line);

        switch (status) {
            case READ_OK:
                LineVectorPush(lv, *line);
                free(line);
                break;
            case READ_ERROR:
                fprintf(stderr, "ERROR %d\n", line->nr);
                LineFree(line);
                free(line);
                break;
            case READ_EMPTY_LINE:
            case READ_COMMENT:
                LineFree(line);
                free(line);
                break;
            case READ_END:
                LineFree(line);
                free(line);
                return;
        }
    }
}*/