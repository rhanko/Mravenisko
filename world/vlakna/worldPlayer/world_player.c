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
    printf("Den cislo: %d\n", *world->pocet_dni);
    for (int i = 0; i < world->plocha->x; ++i) {
        for (int j = 0; j < world->plocha->y; ++j) {
            printf("%s", dajFarbuPolicka(world->plocha->policka[i][j]));

            BOOLEAN je_mravec = F;
            for (int k = 0; k < world->mravce->pocet_mravcov; ++k) {
                MRAVEC mravec = *world->mravce->mravec[k];

                if (*mravec.x == i && *mravec.y == j && *mravec.existuje == T) {
                    printf("*\t");
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
    world_vypis(d->data->world);
    pthread_mutex_unlock(d->data->mutex);

    while(1) {
        //zablokovanie mutexu
        pthread_mutex_lock(d->data->mutex);

        //ukoncenie
        if (*d->data->koniec) {
            return 0;
        }

        //zastavenie
        while(*d->data->pauza == T) {
            pthread_cond_wait(d->pauza, d->data->mutex);
        }

        //pohyb zivych mravcov
        for (int i = 0; i < d->data->world->mravce->pocet_mravcov; ++i) {
            MRAVEC mravec = *d->data->world->mravce->mravec[i];

            if (*mravec.existuje == T) {
                POLICKO policko_mravca = *d->data->world->plocha->policka[*mravec.x][*mravec.y];
                mravec_posunsa(&mravec, (policko_mravca + d->data->world->logika) % 2, d->data->world->plocha->x,d->data->world->plocha->y);
                zmenFarbuPolicka(&policko_mravca);
            }
        }

        //stret 2 alebo viac mravcov TODO: Zabit vsetkych mravcov a znizit pocet existujucich o pocet zabitych, pokial je 0 zivych ukoncit


        //vypisanie sveta s mravcami
        world_vypis(d->data->world);

        //zvysenie dňa
        ++*d->data->world->pocet_dni;

        //odblokovanie mutexu
        pthread_mutex_unlock(d->data->mutex);

        //pauza
        sleep(1);
    }
}