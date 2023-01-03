//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_MRAVCE_H
#define MRAVENISKO_MRAVCE_H

#include "mravec/mravec.h"

typedef struct mravce {
    MRAVEC* *mravec;
    int pocet_mravcov;
} MRAVCE;

MRAVCE mravce_create(int pocet_mravcov);
MRAVCE mravce_random_generate(int pocet_mravcov, int max_x, int max_y);
#endif //MRAVENISKO_MRAVCE_H
