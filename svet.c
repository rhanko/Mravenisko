//
// Created by hanko on 28. 12. 2022.
//
#include "svet.h"

#include <stdlib.h>

//TODO: rozmery, pocet mravcov, logika (priama - 0, nepriama - 1)
SVET world_create(int velkost_x, int velkost_y, int pocet_mravcov, int logika) {
    POLICKO **policka;
    policka = (POLICKO **) malloc(velkost_x * sizeof(POLICKO *));
    for (int i = 0; i < velkost_x; ++i) {
        policka[i] = (POLICKO *) malloc(velkost_y * sizeof(POLICKO));
    }

    MRAVEC *mravce;
    mravce = (MRAVEC *) malloc(pocet_mravcov * sizeof(MRAVEC *));
    int c = 0;

    for (int i = 0; i < velkost_x; ++i) {
        for (int j = 0; j < velkost_y; ++j) {
            policka[i][j] = B;
        }
    }

    for (int i = 0; i < pocet_mravcov; ++i) {
        int cSmer = rand() % 4;
        SMER smer = (cSmer == 0 ? HORE : (cSmer == 1 ? VPRAVO : (cSmer == 2 ? DOLE : VLAVO)));

        mravce[i].x = 0;
        mravce[i].y = 0;
        mravce[i].smer = smer;
    }

    while (c < pocet_mravcov) {
        int x = rand() % velkost_x;
        int y = rand() % velkost_y;

        BOOLEAN bool = F;
        for (int i = 0; i < c; i++) {
            if (mravce[i].x == x && mravce[i].y == y) {
                bool = T;
            }
        }
        if (bool == F) {
            mravce[c].x = x;
            mravce[c].y = y;
            c++;
        }
    }

    SVET svet = {
            (POLICKO **) policka,
            (MRAVEC *) mravce,
            logika
    };

    return svet;
}

int world_start(SVET svet) {

}
