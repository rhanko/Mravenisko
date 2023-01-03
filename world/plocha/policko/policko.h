//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_POLICKO_H
#define MRAVENISKO_POLICKO_H

typedef enum {B, C} POLICKO;

void zmenFarbuPolicka(POLICKO *policko);
char *dajFarbuPolicka(const POLICKO *policko);
void nastavFarbuPolicka(char farba, POLICKO *policko);
#endif //MRAVENISKO_POLICKO_H
