//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_PLOCHA_H
#define MRAVENISKO_PLOCHA_H

#include "policko/policko.h"

typedef struct plocha {
    POLICKO* **policka;
    int x;
    int y;
} PLOCHA;

PLOCHA plocha_create(int velkost_x, int velkost_y);
void plocha_vypis(PLOCHA *plocha);

#endif //MRAVENISKO_PLOCHA_H
