//
// Created by hanko on 3. 1. 2023.
//

#include <stdlib.h>
#include "policko.h"

void zmenFarbuPolicka(POLICKO *policko) {
    *policko = (*policko == B ? C : B);
}

char *dajFarbuPolicka(const POLICKO *policko) {
    return (*policko == B ? "B" : "C");
}

void nastavFarbuPolicka(char farba, POLICKO *policko) {
    if (farba == 'C') {
        *policko = C;
    } else if(farba == 'B') {
        *policko = B;
    } else {
        exit(-1);
    }
}