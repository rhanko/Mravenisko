//
// Created by hanko on 28. 12. 2022.
//

#ifndef MRAVENISKO_WORLD_H
#define MRAVENISKO_WORLD_H

#include "plocha/plocha.h"
#include "mravce/mravec/mravec.h"
#include "../definition.h"
#include "mravce/mravce.h"

typedef struct world {
    char *nazov;
    PLOCHA plocha;
    MRAVCE mravce;
    int logika;
    int pocet_dni;
} WORLD;

WORLD world_create(char *nazov, int velkost_x, int velkost_y, int pocet_mravcov, int logika);
void world_save(char *filename, WORLD *world, int typ);
WORLD world_load(char *filename, char *nazov_sveta, int typ);

#endif //MRAVENISKO_WORLD_H
