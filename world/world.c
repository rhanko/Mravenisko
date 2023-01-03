#include "world.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Funkcia na vytvorenie sveta
 * @param nazov - názov sveta
 * @param velkost_x - pocet policok na x-ovej osi
 * @param velkost_y - pocet policok na y-ovej osi
 * @param pocet_mravcov - celkovo mravcov
 * @param logika - logika, ak je priama daj 0, ak je inverzna daj 1
 * @return vrati svet
 */
WORLD world_create(char *nazov, int velkost_x, int velkost_y, int pocet_mravcov, int logika) {
    POLICKO **policka;
    policka = (POLICKO **) malloc(velkost_x * sizeof(POLICKO *));
    for (int i = 0; i < velkost_x; ++i) {
        policka[i] = (POLICKO *) malloc(velkost_y * sizeof(POLICKO));
    }

    MRAVEC *mravce;
    mravce = (MRAVEC *) malloc(pocet_mravcov * sizeof(MRAVEC *));
    int c = 0;

    for (int i = 0; i < velkost_x; ++i) {
        for (int j = 0; j < velkost_y; ++j) {
            policka[i][j] = B;
        }
    }

    for (int i = 0; i < pocet_mravcov; ++i) {
        int cSmer = rand() % 4;
        SMER smer = (cSmer == 0 ? HORE : (cSmer == 1 ? VPRAVO : (cSmer == 2 ? DOLE : VLAVO)));

        mravce[i].x = 0;
        mravce[i].y = 0;
        mravce[i].smer = smer;
    }

    while (c < pocet_mravcov) {
        int x = rand() % velkost_x;
        int y = rand() % velkost_y;

        BOOLEAN bool = F;
        for (int i = 0; i < c; i++) {
            if (mravce[i].x == x && mravce[i].y == y) {
                bool = T;
            }
        }
        if (bool == F) {
            mravce[c].x = x;
            mravce[c].y = y;
            c++;
        }
    }

    WORLD svet = {
            nazov,
            (POLICKO **) policka,
            velkost_x,
            velkost_y,
            (MRAVEC *) mravce,
            pocet_mravcov,
            logika,
            0
    };

    return svet;
}

/**
 * Funkcia na ulozenie sveta alebo vzoru
 * @param filename subor, kde sa ma ulozit
 * @param world dany svet / vzor, ktory sa ma ulozit do subora
 * @param typ ci sa ma ulozit vzor (1) alebo svet (0)
 * @return vrati 0, ak sa to ulozi spravne
 */
int world_save(char *filename, WORLD world, int typ) {
    FILE *subor = fopen(filename, "w");

    if (subor == NULL) {
        printf("Chyba! Súbor sa nepodarilo otvoriť.\n");
        return -1;
    }

    if(typ == 0) {
        fprintf(subor, "Názov sveta: %s {\n", world.nazov);
    } else {
        fprintf(subor, "Názov vzoru: %s {\n", world.nazov);
    }
    fprintf(subor, "\tVeľkost X: %d\n", world.x);
    fprintf(subor, "\tVeľkost Y: %d\n", world.y);

    if(typ == 0) {
        fprintf(subor, "\tPočet mravcov: %d\n", world.pocet_mravcov);
        fprintf(subor, "\tLogika: %d\n", world.logika);
        fprintf(subor, "\tPočet dní: %d\n", world.pocet_dni);
    }
    fprintf(subor, "\tPolíčka {\n");

    for (int i = 0; i < world.x; ++i) {
        for (int j = 0; j < world.y; ++j) {
            if (world.policka[i][j] == C) {
                fprintf(subor, "\t\t%d, %d: C\n", i, j);
            } else {
                fprintf(subor, "\t\t%d, %d: B\n", i, j);
            }
        }
    }
    fprintf(subor, "\t}\n");

    if(typ == 0) {
        fprintf(subor, "\tMravce {\n");

        for (int i = 0; i < world.pocet_mravcov; ++i) {
            SMER sSmer = world.mravce[i].smer;
            char *smer = (sSmer == HORE ? "HORE" : (sSmer == VPRAVO ? "VPRAVO" : (sSmer == DOLE ? "DOLE" : "VLAVO")));

            fprintf(subor, "\t\t%d, %d, %s\n", world.mravce[i].x, world.mravce[i].y, smer);
        }
        fprintf(subor, "\t}\n");
    }
    fprintf(subor, "}\n");

    fclose(subor);
    return 0;
}

/**
 * Funkcia pre nacitanie sveta / vzora
 * @param filename nazov suboru, odkial sa ma nacitat
 * @param nazov_sveta ako sa vola daný svet / vzor
 * @param typ ci ide o vzor (1) alebo svet (0)
 * @return vráti svet, ak sa tam nachadza
 */
WORLD world_load(char *filename, char *nazov_sveta, int typ) {
    FILE *subor = fopen(filename, "r");

    if (subor == NULL) {
        printf("Chyba! Súbor sa nepodarilo otvoriť.\n");
        exit(1);
    }

    char *NAZOV;
    if (typ == 0) {
        NAZOV = "Názov sveta";
    } else {
        NAZOV = "Názov vzoru";
    }

    char *meno_sveta = "";

    //hlada sa svet/ vzor na zaklade hladaneho nazvu
    char line[100];
    while (fgets(line, 100, subor) != NULL) {
        char *cSvet = malloc(12);
        strncpy(cSvet, line, 12);

        char *nazov_sveta_najdeny;
        if (strcmp(cSvet, NAZOV) == 0) {
            int odkial = 14;
            int pokial = 0;

            for (int i = odkial; i < 100; ++i) {
                if (line[i] == '{') {
                    pokial = i - 1;
                    break;
                }
            }

            nazov_sveta_najdeny = malloc(pokial - odkial);
            strncpy(nazov_sveta_najdeny, line + odkial, pokial - odkial);

            if (strcmp(nazov_sveta, nazov_sveta_najdeny) == 0) {
                meno_sveta = nazov_sveta_najdeny;
                break;
            }
        }
    }

    //Ak sa nenajde svet/ vzor
    if (strcmp(meno_sveta,nazov_sveta) != 0) {
        exit(2);
    }

    //Pocet policok v x osi
    fgets(line, 100, subor);
    char *cX = malloc(5);
    strcpy(cX, line + 13);
    int x = atoi(cX);

    //Pocet policok v y osi
    fgets(line, 100, subor);
    char *cY = malloc(5);
    strcpy(cY, line + 13);
    int y = atoi(cY);

    //Pocet mravcov, logiky a dni
    char *cPocet_mravcov = malloc(5);
    int pocet_mravcov = 0;
    char *cLogika = malloc(5);
    int logika = 0;
    char *cPocet_dni = malloc(5);
    int pocet_dni = 0;

    //alokacia iba ak je to svet nie vzor
    if (typ == 0) {
        fgets(line, 100, subor);
        strcpy(cPocet_mravcov, line + 17);
        pocet_mravcov = atoi(cPocet_mravcov);

        fgets(line, 100, subor);
        strcpy(cLogika, line + 9);
        logika = atoi(cLogika);

        fgets(line, 100, subor);
        strcpy(cPocet_dni, line + 14);
        pocet_dni = atoi(cPocet_dni);
    }

    //Vynechanie riadku
    fgets(line, 100, subor);


    //POLICKA
    POLICKO **policka;
    policka = (POLICKO **) malloc(x * sizeof(POLICKO *));
    for (int i = 0; i < x; ++i) {
        policka[i] = (POLICKO *) malloc(y * sizeof(POLICKO));
    }

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            fgets(line, 100, subor);
            unsigned int position = strcspn(line, "\n") - 1;
            policka[i][j] = (line[position] == 'C' ? C : B);
        }
    }

    //MRAVCE
    MRAVEC *mravce;
    mravce = (MRAVEC *) malloc(pocet_mravcov * sizeof(MRAVEC *));

    if (typ == 0) {
        for (int i = 0; i < 2; ++i) {
            fgets(line, 100, subor);
        }

        for (int i = 0; i < pocet_mravcov; ++i) {
            fgets(line, 100, subor);
            int odkial = 2;
            int pokial = 0;

            for (int j = odkial; j < 100; ++j) {
                if (line[j] == ',') {
                    pokial = j;
                    break;
                }
            }
            char *cxm = malloc(pokial - odkial);
            strncpy(cxm,line + odkial, pokial - odkial);
            int xm = atoi(cxm);

            odkial = pokial + 2;

            for (int j = odkial; j < 100; ++j) {
                if (line[j] == ',') {
                    pokial = j;
                    break;
                }
            }
            char *cym = malloc(pokial - odkial);
            strncpy(cym, line + odkial, pokial - odkial);
            int ym = atoi(cym);

            odkial = pokial + 2;

            char *ctm = malloc(10);
            strcpy(ctm, line + odkial);

            mravce[i].x = xm;
            mravce[i].y = ym;
            mravce[i].smer = (strcmp(ctm, "HORE\n") == 0 ? HORE : (strcmp(ctm, "VPRAVO\n") == 0 ? VPRAVO : (strcmp(ctm, "DOLE") == 0 ? DOLE : VLAVO)));
        }
    }

    fclose(subor);

    WORLD world = {
            meno_sveta,
            policka,
            x,
            y,
            mravce,
            pocet_mravcov,
            logika,
            pocet_dni
    };

    return world;
}

int world_play() {


    return 0;
}