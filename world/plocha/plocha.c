//
// Created by hanko on 3. 1. 2023.
//
#include <malloc.h>
#include "plocha.h"

/**
 * Funkcia na vytvorenie plochy
 * @param velkost_x pocet policok po x-ovej osi
 * @param velkost_y pocet policok po y-ovej osi
 * @return vrati plochu
 */
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
            policka,
            velkost_x,
            velkost_y
    };

    return plocha;
}

/**
 * Procedura na vypisanie vzoru
 * @param plocha plocha sveta
 */
void plocha_vypis(PLOCHA *plocha) {
    for (int i = 0; i < plocha->x; ++i) {
        for (int j = 0; j < plocha->y; ++j) {
            printf("%s\t", (plocha->policka[i][j] == B ? "B" : "C"));
        }
        printf("\n");
    }
}