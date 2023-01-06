//
// Created by hanko on 6. 1. 2023.
//
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include "client.h"

/**
 * Vlakno, ktore sa vytvori pri pripojeni sa klienta na server
 * @param data data, s ktorymi pracuje
 * @return 0
 */
void *client(void *data) {
    VLAKNO_DATA *d = (VLAKNO_DATA *) data;

    //nacita moznost, ktoru chce uzivatel spravit
    read(d->client_socket, &d->co_robit, sizeof(int));
    printf("Bola vybratá možnosť: %d\n", d->co_robit);

    if (d->co_robit == 0) {
        //precitanie mena vzoru
        char *vzor_meno = malloc(sizeof(char) * 100);
        read(d->client_socket, vzor_meno, sizeof(char)*100);

        //nacitanie mena vzoru
        pthread_mutex_lock(d->mutex);
        int existuje = world_existuje("vzory.txt", vzor_meno, 1);
        if (existuje == 0) {
            d->world = world_load("vzory.txt", vzor_meno, 1);
        }
        pthread_mutex_unlock(d->mutex);

        //ak existuje posle klientovi 0 a nasledne vzor, ak neexistuje tak posle klientovi -1
        write(d->client_socket, &existuje, sizeof (int));
        if (existuje == 0) {
            write(d->client_socket, &d->world, sizeof(WORLD));
        }

    } else {
        read(d->client_socket, &d->world, sizeof(WORLD));
        pthread_mutex_lock(d->mutex);
        world_save("vzory.txt", &d->world, 1);
        pthread_mutex_unlock(d->mutex);
    }

    //uzavretie socketu klienta <unistd.h>
    printf("Klient ukoncil komunikaciu.\n");
    close(d->client_socket);
    return 0;
}
