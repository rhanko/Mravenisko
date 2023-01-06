//
// Created by hanko on 6. 1. 2023.
//
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <string.h>
#include "client.h"

/**
 * Vlakno, ktore sa vytvori pri pripojeni sa klienta na server
 * @param data data, s ktorymi pracuje
 * @return 0
 */
void *client(void *data) {
    VLAKNO_DATA *d = (VLAKNO_DATA *) data;

    //nacita moznost, ktoru chce uzivatel spravit
    read(d->client_socket, &d->co_robit, sizeof(int));                                  //1
    printf("Bola vybratá možnosť: %d\n", d->co_robit);

    WORLD world;

    if (d->co_robit == 0) {
        //precitanie mena vzoru
        char *vzor_meno = malloc(sizeof(char) * 100);
        read(d->client_socket, vzor_meno, sizeof(char)*100);                            //2

        //nacitanie mena vzoru
        pthread_mutex_lock(d->mutex);
        int existuje = world_existuje("vzory.txt", vzor_meno, 1);

        if (existuje == 0) {
            world = world_load("vzory.txt", vzor_meno, 1);
        }
        pthread_mutex_unlock(d->mutex);

        //ak existuje posle klientovi 0 a nasledne vzor, ak neexistuje tak posle klientovi -1
        write(d->client_socket, &existuje, sizeof (int));                                   //3
        if (existuje == 0) {
            write(d->client_socket, &world.plocha.x, sizeof(int));                          //4
            write(d->client_socket, &world.plocha.y, sizeof(int));                          //5


            for (int i = 0; i < world.plocha.x; ++i) {
                for (int j = 0; j < world.plocha.y; ++j) {
                    write(d->client_socket, &world.plocha.policka[i][j], sizeof(POLICKO));  //6
                }
            }
        }


    } else {
        //nacita do world svet od klienta
        char nazov[100];
        read(d->client_socket, &nazov, sizeof(char)*100);                               //2

        int x;
        read(d->client_socket, &x, sizeof(int));                                        //3

        int y;
        read(d->client_socket, &y, sizeof(int));                                        //4

        world = world_create(nazov, x, y, 0, 0);

            for (int i = 0; i < x; ++i) {
                for (int j = 0; j < y; ++j) {
                    POLICKO policko;
                    read(d->client_socket, &policko, sizeof(POLICKO));                  //5
                    world.plocha.policka[i][j] = policko;
                }
            };
        char meno_sveta[100];
        strcpy(meno_sveta,world.nazov);
        pthread_mutex_lock(d->mutex);

        //ulozi svet
        world_save("vzory.txt", &world, 1);
        pthread_mutex_unlock(d->mutex);

        //vrati ze je ulozeny
        write(d->client_socket, 0, sizeof(int));                                            //6
    }

    //uzavretie socketu klienta <unistd.h>
    printf("Klient ukoncil komunikaciu.\n");
    close(d->client_socket);
    return 0;
}
