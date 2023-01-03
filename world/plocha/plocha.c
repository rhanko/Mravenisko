//
// Created by hanko on 3. 1. 2023.
//
#include <malloc.h>
#include "plocha.h"

PLOCHA plocha_create(int velkost_x, int velkost_y) {
    POLICKO **policka;
    policka = (POLICKO **) malloc(velkost_x * sizeof(POLICKO *));
    for (int i = 0; i < velkost_x; ++i) {
        policka[i] = (POLICKO *) malloc(velkost_y * sizeof(POLICKO));
    }

    for (int i = 0; i < velkost_x; ++i) {
        for (int j = 0; j < velkost_y; ++j) {
            policka[i][j] = B;
        }
    }

    PLOCHA plocha = {
            &policka,
            velkost_x,
            velkost_y
    };

    return plocha;
}

void plocha_vypis(PLOCHA *plocha) {
    for (int i = 0; i < plocha->x; ++i) {
        for (int j = 0; j < plocha->y; ++j) {
            printf("%s\t", (plocha->policka[i][j] == B ? "B" : "C"));
        }
        printf("\n");
    }
}