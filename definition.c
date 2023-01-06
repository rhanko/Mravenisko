//
// Created by hanko on 28. 12. 2022.
//

#include "definition.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void printError(char *str) {
    if (errno != 0) {
        perror(str);
    }
    else {
        fprintf(stderr, "%s\n", str);
    }
    exit(EXIT_FAILURE);
}
