//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include <string.h>
#include "mravec.h"

MRAVEC mravec_create_random(int x, int y) {
    int cSmer = rand() % 4;
    SMER smer = (cSmer == 0 ? HORE : (cSmer == 1 ? VPRAVO : (cSmer == 2 ? DOLE : VLAVO)));

    MRAVEC mravec = {
            x,
            y,
            smer,
            T
    };

    return mravec;
}

char *mravec_daj_smer(MRAVEC *mravec) {
    MRAVEC m = *mravec;

    switch (m.smer) {
        case HORE:
            return "HORE";
        case VPRAVO:
            return "VPRAVO";
        case VLAVO:
            return "VLAVO";
        case DOLE:
            return "DOLE";
    }
}

void mravec_nastav_smer(MRAVEC *mravec, char *smer) {
    if (strcmp(smer, "HORE") == 0) {
        mravec->smer = HORE;
    } else
    if (strcmp(smer, "DOLE") == 0) {
        mravec->smer = HORE;
    } else
    if (strcmp(smer, "VPRAVO") == 0) {
        mravec->smer = HORE;
    } else
    if (strcmp(smer, "VLAVO") == 0) {
        mravec->smer = HORE;
    } else {
        exit(1);
    }
}

void mravec_posunsa(MRAVEC *mravec, int otocenie, int max_x, int max_y) {
    MRAVEC m = *mravec;
    if (m.existuje == T) {
        switch (m.smer) {
            case HORE:
                (m.y == max_y ? m.y = 0 : ++m.y);
                if (otocenie == 0) {
                    m.smer = VPRAVO;
                } else {
                    m.smer = VLAVO;
                }
                break;
            case VPRAVO:
                (m.x == max_x ? m.x = 0 : ++m.x);
                if (otocenie == 0) {
                    m.smer = DOLE;
                } else {
                    m.smer = HORE;
                }
                break;
            case VLAVO:
                (m.x == 0 ? m.x = max_x : --m.x);
                if (otocenie == 0) {
                    m.smer = HORE;
                } else {
                    m.smer = DOLE;
                }
                break;
            case DOLE:
                (m.y == 0 ? m.y = max_y : --m.y);
                if (otocenie == 0) {
                    m.smer = VLAVO;
                } else {
                    m.smer = VPRAVO;
                }
                break;
        }
    }
}

void mravec_zomri(MRAVEC *mravec) {
    mravec->existuje = F;
}
