//
// Created by hanko on 3. 1. 2023.
//

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "world_player.h"

/**
 * Procedura, ktora vypise farbu policok a umiestnenie mravcov
 * @param world svet, ktory sa ma vypisat
 */
void world_vypis(WORLD *world) {
    printf("Den cislo: %d\n", world->pocet_dni);
    for (int i = 0; i < world->plocha.x; ++i) {
        for (int j = 0; j < world->plocha.y; ++j) {
            printf("%s", dajFarbuPolicka(&world->plocha.policka[i][j]));

            BOOLEAN je_mravec = F;
            for (int k = 0; k < world->mravce.pocet_mravcov; ++k) {
                MRAVEC mravec = world->mravce.mravec[k];

                if (mravec.x == i && mravec.y == j && mravec.existuje == T) {
                    char cSmer;

                    POLICKO *policko = &world->plocha.policka[i][j];

                    if (world->logika == 0 ) {
                        if (*policko == B) {
                            if (mravec.smer == HORE) {
                                cSmer = 'U';
                            } else if (mravec.smer == DOLE) {
                                cSmer = 'D';
                            } else if (mravec.smer == VPRAVO) {
                                cSmer = 'R';
                            } else {
                                cSmer = 'L';
                            }
                        } else {
                            if (mravec.smer == HORE) {
                                cSmer = 'D';
                            } else if (mravec.smer == DOLE) {
                                cSmer = 'H';
                            } else if (mravec.smer == VPRAVO) {
                                cSmer = 'L';
                            } else {
                                cSmer = 'R';
                            }
                        }
                    } else {
                        if (*policko == C) {
                            if (mravec.smer == HORE) {
                                cSmer = 'U';
                            } else if (mravec.smer == DOLE) {
                                cSmer = 'D';
                            } else if (mravec.smer == VPRAVO) {
                                cSmer = 'R';
                            } else {
                                cSmer = 'L';
                            }
                        } else {
                            if (mravec.smer == HORE) {
                                cSmer = 'D';
                            } else if (mravec.smer == DOLE) {
                                cSmer = 'H';
                            } else if (mravec.smer == VPRAVO) {
                                cSmer = 'L';
                            } else {
                                cSmer = 'R';
                            }
                        }
                    }

                    printf("%c\t", cSmer);
                    je_mravec = T;
                    break;
                }
            }

            if (je_mravec == F) {
                printf("\t");
            }
        }
        printf("\n");
    }
}

/**
 * Vlakno, ktore spravuje svet
 * @param data
 * @return
 */
void *world_player(void *data) {
    WORLD_PLAYER_DATA *d = (WORLD_PLAYER_DATA *) data;

    //vypisanie sveta;
    pthread_mutex_lock(d->data->mutex);
    if (*d->data->pauza == T) {
        pthread_cond_wait(d->pauza, d->data->mutex);
    }

    world_vypis(d->data->world);
    pthread_mutex_unlock(d->data->mutex);

    sleep(5);

    while(1) {
        //zablokovanie mutexu
        pthread_mutex_lock(d->data->mutex);

        //zastavenie
        while(*d->data->pauza == T || d->data->world->mravce.pocet_zivych_mravcov == 0) {
            //ukoncenie
            if (*d->data->koniec == T) {
                return 0;
            }

            if (d->data->world->mravce.pocet_zivych_mravcov == 0) {
                printf("Všetky mravce umreli.\n");
            }

            pthread_cond_wait(d->pauza, d->data->mutex);
        }

        //pohyb zivych mravcov
        for (int i = 0; i < d->data->world->mravce.pocet_mravcov; ++i) {
            MRAVEC *mravec = &d->data->world->mravce.mravec[i];

            if (mravec->existuje == T) {
                POLICKO *policko_mravca = &d->data->world->plocha.policka[mravec->x][mravec->y];
                int cislo = 0;
                if (d->data->world->plocha.policka[mravec->x][mravec->y] != B) {
                    cislo++;
                }
                if (d->data->world->logika == 1) {
                    cislo++;
                }
                mravec_posunsa(mravec, cislo % 2, d->data->world->plocha.x,d->data->world->plocha.y);
                zmenFarbuPolicka(policko_mravca);

            }
        }

        //stret 2 alebo viac mravcov, oznacenie vsetkych

        MRAVEC *mravce[d->data->world->mravce.pocet_zivych_mravcov];
        int pocet_zabitych_mravcov = 0;
        for (int i = 0; i < d->data->world->mravce.pocet_mravcov; ++i) {
            MRAVEC *mravecA = &d->data->world->mravce.mravec[i];

            if(mravecA->existuje == T) {
                for (int j = 0; j < d->data->world->mravce.pocet_mravcov; ++j) {
                    if(i != j) {
                        MRAVEC *mravecB = &d->data->world->mravce.mravec[j];

                        if(mravecB->existuje == T && mravecA->x == mravecB->x && mravecA->y == mravecB->y) {
                            mravce[pocet_zabitych_mravcov] = mravecA;
                            pocet_zabitych_mravcov++;
                        }
                    }
                }
            }
        }

        //zabitie mravcov
        for (int i = 0; i < pocet_zabitych_mravcov; ++i) {
            mravec_zomri(mravce[i]);
        }

        //zvysenie dňa
        ++d->data->world->pocet_dni;

        //vypisanie sveta s mravcami
        world_vypis(d->data->world);

        //odblokovanie mutexu
        pthread_mutex_unlock(d->data->mutex);

        //pauza
        sleep(5);
    }
}