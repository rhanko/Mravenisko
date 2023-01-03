#include "world.h"
#include "plocha/plocha.h"
#include "mravce/mravec/mravec.h"
#include "mravce/mravce.h"

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
    PLOCHA plocha = plocha_create(velkost_x, velkost_y);

    MRAVCE mravce = mravce_create(pocet_mravcov);

    WORLD svet = {
            nazov,
            &plocha,
            &mravce,
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
    fprintf(subor, "\tVeľkost X: %d\n", world.plocha->x);
    fprintf(subor, "\tVeľkost Y: %d\n", world.plocha->y);

    if(typ == 0) {
        fprintf(subor, "\tPočet mravcov: %d\n", world.mravce->pocet_mravcov);
        fprintf(subor, "\tLogika: %d\n", world.logika);
        fprintf(subor, "\tPočet dní: %d\n", world.pocet_dni);
    }
    fprintf(subor, "\tPolíčka {\n");

    for (int i = 0; i < world.plocha->x; ++i) {
        for (int j = 0; j < world.plocha->y; ++j) {
            fprintf(subor, "\t\t%d, %d: %s\n", i, j, dajFarbuPolicka(world.plocha->policka[i][j]));
        }
    }
    fprintf(subor, "\t}\n");

    if(typ == 0) {
        fprintf(subor, "\tMravce {\n");

        for (int i = 0; i < world.mravce->pocet_mravcov; ++i) {
            fprintf(subor, "\t\t%d, %d, %s\n", world.mravce->mravec[i]->x, world.mravce->mravec[i]->y, mravec_daj_smer(world.mravce->mravec[i]));
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

    //alokacia iba ak je to svet a nie vzor
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


    //PLOCHA
    PLOCHA plocha = plocha_create(x,y);

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            fgets(line, 100, subor);
            unsigned int position = strcspn(line, "\n") - 1;
            nastavFarbuPolicka(line[position], plocha.policka[i][j]);
        }
    }

    //MRAVCE
    MRAVCE mravce = mravce_create(pocet_mravcov);

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
            ctm = ctm -1;
            mravce.mravec[i]->x = xm;
            mravce.mravec[i]->y = ym;
            mravce.mravec[i]->existuje = T;
            mravec_nastav_smer(mravce.mravec[i], ctm);
        }
    }

    fclose(subor);

    WORLD world = {
            meno_sveta,
            &plocha,
            &mravce,
            logika,
            pocet_dni
    };

    return world;
}