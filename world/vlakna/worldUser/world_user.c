//
// Created by hanko on 4. 1. 2023.
//

#include <stdio.h>
#include <pthread.h>
#include "world_user.h"
#include "../worldPlayer/world_player.h"

int getInputNumber() {
    int cislo = 0, ch;

    do {
        ch = getchar();

        if (ch >= '0' && ch <= '9') {
            printf("%c", ch);
            cislo = cislo * 10 + (ch - 48);
        }
    } while (ch == '\n');

    return (cislo > 0 ? cislo : 1);
}

WORLD new_world() {
    printf("Zadaj názov sveta, ktorý chceš vytvoriť: \n");
    char nazov_sveta[100];
    gets(nazov_sveta);

    printf("Zadaj veľkosť x-ovej osi: \n");
    int max_x = getInputNumber();

    printf("Zadaj veľkosť y-ovej osi: \n");
    int max_y = getInputNumber();

    printf("Zadaj počet mravcov: \n");
    int pocet_mravcov = getInputNumber();

    printf("Vyber logiku: \n1.) priama\n2.)inverzná");
    int logika = getInputNumber() - 1;

    WORLD world = world_create(nazov_sveta, max_x, max_y, pocet_mravcov, logika);

    return world;
}

WORLD load_world(char* filename, int typ) {
    printf("Zadaj názov sveta, ktorý sa má načítať: \n");
    char nazov_sveta[100];
    gets(nazov_sveta);

    WORLD world = world_load(filename, nazov_sveta, typ);

    return world;
}

int menu_jedna() {
    int input;

    printf("Vyberte si 1 z moznosti:\n");
    printf("1.) Vytvoriť nový svet.\n");
    printf("2.) Načítať svet.\n");
    printf("3.) Načítať vzor so servera.\n");
    printf("5.) Ukončiť.\n");

    input = getInputNumber();
    return input;
}

int menu_dva() {
    int input;

    printf("Vyberte si 1 z moznosti:\n");
    printf("1.) Spustiť svet. / Zastaviť svet.\n");
    printf("2.) Uložiť svet.\n");
    printf("3.) Uložiť vzor na server.\n");
    printf("5.) Ukončiť svet (bez uloženia).\n");

    input = getInputNumber();
    return input;
}

void *world_user(void *data) {
    WORLD_USER_DATA *d = (WORLD_USER_DATA *) data;

    int vyber;
    do {
        vyber = menu_jedna();

        WORLD world;

        switch (vyber) {
            case 1:
                world = new_world();
                break;
            case 2:
                world = load_world("worlds.txt", 0);
                break;
            case 3:
                //TODO napravit to, na presmerovanie na server a odtial stiahnut :/
                world = load_world("vzory.txt", 1);
                break;
            default: break;
        }

        if (vyber == 1 || vyber == 2) {

            pthread_mutex_lock(d->data->mutex);

            WORLD_PLAYER_DATA pd = {
                    d->data,
                    d->pauza,
                    d->pokracuj
            };

            pthread_t world_player_thread;
            pthread_create(&world_player_thread, NULL, &world_player, &pd);
            pthread_join(world_player_thread, NULL);

            pthread_mutex_unlock(d->data->mutex);
            int vyber2;
            do {
                vyber2 = menu_dva();


            } while (vyber2 != 5);

        }

    } while (vyber != 5);

    return 0;
}