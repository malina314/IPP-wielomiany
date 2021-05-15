#include "parse.h"
#include "line.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

static inline bool Prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

Line parse(const char *str) {
    if (isalpha(str[0])) { // komenda
        Command command =
    }
    else { // wielomian

    }
}