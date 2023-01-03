//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include "mravce.h"

MRAVCE mravce_create(int pocet_mravcov) {
    MRAVEC *mravce;
    mravce = (MRAVEC *) malloc(pocet_mravcov * sizeof(MRAVEC *));
    MRAVCE mmravce = {
            &mravce,
            pocet_mravcov
    };

    return mmravce;
}

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
