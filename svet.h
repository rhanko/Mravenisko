//
// Created by hanko on 28. 12. 2022.
//

#ifndef MRAVENISKO_SVET_H
#define MRAVENISKO_SVET_H

typedef enum {B, C} POLICKO;
typedef enum {T, F} BOOLEAN;
typedef enum {HORE, VPRAVO, DOLE, VLAVO} SMER;

typedef struct mravec {
    int x;
    int y;
    SMER smer;
} MRAVEC;

typedef struct svet {
    POLICKO **policka;
    MRAVEC *mravce;
    int logika;
} SVET;



#endif //MRAVENISKO_SVET_H
