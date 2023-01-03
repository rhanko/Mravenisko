//
// Created by hanko on 28. 12. 2022.
//

#ifndef MRAVENISKO_WORLD_H
#define MRAVENISKO_WORLD_H

typedef enum {B, C} POLICKO;
typedef enum {T, F} BOOLEAN;
typedef enum {HORE, VPRAVO, DOLE, VLAVO} SMER;

typedef struct mravec {
    int x;
    int y;
    SMER smer;
} MRAVEC;

typedef struct world {
    char *nazov;
    POLICKO **policka;
    int x;
    int y;
    MRAVEC *mravce;
    int pocet_mravcov;
    int logika;
    int pocet_dni;
} WORLD;

WORLD world_create(char *nazov, int velkost_x, int velkost_y, int pocet_mravcov, int logika);
int world_play(WORLD world);
int world_pause(WORLD world);
int world_save(char *filename, WORLD world, int typ);
WORLD world_load(char *filename, char *nazov_sveta, int typ);



#endif //MRAVENISKO_WORLD_H
