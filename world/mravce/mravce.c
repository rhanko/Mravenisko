//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include "mravce.h"

/**
 * Funkcia na vytvorenie mravcov
 * @param pocet_mravcov celkovy pocet mravcov, ktory sa maju vytvorit
 * @return vrati mravce
 */
MRAVCE mravce_create(int pocet_mravcov) {
    MRAVEC *mravce;
    mravce = (MRAVEC *) malloc(pocet_mravcov * sizeof(MRAVEC *));
    MRAVCE mmravce = {
            &mravce,
            pocet_mravcov,
            &pocet_mravcov
    };

    return mmravce;
}

/**
 * Funkcia na vytvorenie nahodnej polohy a smeru mravcov
 * @param pocet_mravcov celkovy pocet mravcov co sa ma vytvorit
 * @param max_x pocet policok v x-ovej osi
 * @param max_y pocet policok v y-ovej osi
 * @return vrati mravce
 */
MRAVCE mravce_random_generate(int pocet_mravcov, int max_x, int max_y) {
    MRAVCE mravce = mravce_create(pocet_mravcov);
    int c = 0;

    while (c < pocet_mravcov) {
        *mravce.mravec[c] = mravec_create_random(rand() % max_x, rand() % max_y);

        BOOLEAN bool = F;
        for (int i = 0; i < c; i++) {
            if (mravce.mravec[i]->x == mravce.mravec[c]->x && mravce.mravec[i]->y == mravce.mravec[c]->y) {
                bool = T;
            }
        }
        if (bool == F) {
            c++;
        }
    }

    return mravce;
}
