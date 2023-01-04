//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include "policko.h"

/**
 * Procedura na zmenu farby policka
 * @param policko policko, ktore chceme zmenit
 */
void zmenFarbuPolicka(POLICKO *policko) {
    *policko = (*policko == B ? C : B);
}

/**
 * Funkcia na ziskanie farby v podobe stringu
 * @param policko policko, z ktoreho chceme ziskat informaciu
 * @return vrati farbu policka
 */
char *dajFarbuPolicka(const POLICKO *policko) {
    return (*policko == B ? "B" : "C");
}

/**
 * Procedura na nastavenie farby
 * @param farba nazov farby
 * @param policko policko, ktoremu farbu chceme zmenit
 */
void nastavFarbuPolicka(char farba, POLICKO *policko) {
    if (farba == 'C') {
        *policko = C;
    } else if(farba == 'B') {
        *policko = B;
    } else {
        exit(-1);
    }
}