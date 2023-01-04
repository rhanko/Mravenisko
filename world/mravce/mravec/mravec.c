//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include <string.h>
#include "mravec.h"

/**
 * Funkcia na vytvorenie nahodneho smeru mravca
 * @param x poloha na x-ovej osi
 * @param y poloha na y-ovej osi
 * @return vytvoreny mravec
 */
MRAVEC mravec_create_random(int x, int y) {
    int cSmer = rand() % 4;
    SMER smer = (cSmer == 0 ? HORE : (cSmer == 1 ? VPRAVO : (cSmer == 2 ? DOLE : VLAVO)));

    MRAVEC mravec = {
            &x,
            &y,
            &smer,
            T
    };

    return mravec;
}

/**
 * Funkcia vrati smer mravca v tvare stringu
 * @param mravec mravec, ktoreho smer chceme v stringu
 * @return nazov smeru
 */
char *mravec_daj_smer(MRAVEC *mravec) {
    switch (*mravec->smer) {
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

/**
 * Procedura na nastavenie smeru pre mravca
 * @param mravec mravec, ktoremu nastavujeme smer
 * @param smer smer, ktory mu chceme nastavit v tvare stringu
 */
void mravec_nastav_smer(MRAVEC *mravec, char *smer) {
    if (strcmp(smer, "HORE") == 0) {
        *mravec->smer = HORE;
    } else
    if (strcmp(smer, "DOLE") == 0) {
        *mravec->smer = HORE;
    } else
    if (strcmp(smer, "VPRAVO") == 0) {
        *mravec->smer = HORE;
    } else
    if (strcmp(smer, "VLAVO") == 0) {
        *mravec->smer = HORE;
    } else {
        exit(1);
    }
}

/**
 * Procedura na presuvanie mravca
 * @param mravec mravec, ktoreho presuvame
 * @param otocenie ako sa ma tocit, 0 ak v smere hodinovych ruciciek, 1 v protismere hodinovych ruciciek
 * @param max_x pocet policok na x-ovej osi
 * @param max_y pocet policok na y-ovej osi
 */
void mravec_posunsa(MRAVEC *mravec, unsigned int otocenie, int max_x, int max_y) {
    if (mravec->existuje == T) {
        switch (*mravec->smer) {
            case HORE:
                (*mravec->y == max_y ? *mravec->y = 0 : ++*mravec->y);
                if (otocenie == 0) {
                    *mravec->smer = VPRAVO;
                } else {
                    *mravec->smer = VLAVO;
                }
                break;
            case VPRAVO:
                (*mravec->x == max_x ? *mravec->x = 0 : ++*mravec->x);
                if (otocenie == 0) {
                    *mravec->smer = DOLE;
                } else {
                    *mravec->smer = HORE;
                }
                break;
            case VLAVO:
                (*mravec->x == 0 ? *mravec->x = max_x : --*mravec->x);
                if (otocenie == 0) {
                    *mravec->smer = HORE;
                } else {
                    *mravec->smer = DOLE;
                }
                break;
            case DOLE:
                (*mravec->y == 0 ? *mravec->y = max_y : --*mravec->y);
                if (otocenie == 0) {
                    *mravec->smer = VLAVO;
                } else {
                    *mravec->smer = VPRAVO;
                }
                break;
        }
    }
}

/**
 * Procedura, ktora zabije mravca
 * @param mravec mravec, s ktoreho sa to tyka
 */
void mravec_zomri(MRAVEC *mravec) {
    *mravec->existuje = F;
}
