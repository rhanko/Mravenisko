//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_MRAVEC_H
#define MRAVENISKO_MRAVEC_H

#include "../../../definition.h"

typedef enum {HORE, VPRAVO, DOLE, VLAVO} SMER;

typedef struct mravec {
    int *x;
    int *y;
    SMER *smer;
    BOOLEAN *existuje;
} MRAVEC;

MRAVEC mravec_create_random(int x, int y);
char *mravec_daj_smer(MRAVEC *mravec);
void mravec_nastav_smer(MRAVEC *mravec, char *smer);
void mravec_posunsa(MRAVEC *mravec, unsigned int otocenie, int max_x, int max_y);
void mravec_zomri(MRAVEC *mravec);

#endif //MRAVENISKO_MRAVEC_H
