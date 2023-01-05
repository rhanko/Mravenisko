//
// Created by hanko on 4. 1. 2023.
//

#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include "world_user.h"

int getInputNumber() {
    int cislo = 0, ch;

    do {
        ch = getchar();

        if (ch >= '0' && ch <= '9') {
            cislo = cislo * 10 + (ch - 48);
        }
    } while (ch != '\n');

    return (cislo > 0 ? cislo : 1);
}

WORLD new_world() {
    printf("Zadaj názov sveta, ktorý chceš vytvoriť: \n");
    char *nazov_sveta = malloc(sizeof (char) * 100);
    scanf("%s",nazov_sveta);
    getchar();
    printf("Zadaj veľkosť x-ovej osi: \n");
    int max_x;
    max_x = getInputNumber();

    printf("Zadaj veľkosť y-ovej osi: \n");
    int max_y;
    max_y = getInputNumber();

    printf("Zadaj počet mravcov: \n");
    int pocet_mravcov;
    pocet_mravcov =getInputNumber();

    printf("Vyber logiku: \n1.) priama\n2.) inverzná\n");
    int logika;
    logika = getInputNumber();

    WORLD world = world_create(nazov_sveta, max_x, max_y, pocet_mravcov, logika-1);

    return world;
}

WORLD load_world(char* filename, int typ) {
    printf("Zadaj názov sveta, ktorý sa má načítať: \n");
    char *nazov_sveta = malloc(sizeof (char) * 100);
    gets(nazov_sveta);

    WORLD world = world_load(filename, nazov_sveta, typ);

    return world;
}

void save_world(char *filename, int typ, WORLD *world) {
    printf("Zadaj názov sveta, pre uloženie: \n");
    char *nazov_sveta = malloc(sizeof (char) * 100);
    gets(nazov_sveta);

    world->nazov = nazov_sveta;
    world_save(filename, world, typ);

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

        switch (vyber) {
            case 1:
                pthread_mutex_lock(d->data->mutex);
                *d->data->world = new_world();
                pthread_mutex_unlock(d->data->mutex);
                break;
            case 2:
                pthread_mutex_lock(d->data->mutex);
                *d->data->world = load_world("worlds.txt", 0);
                pthread_mutex_unlock(d->data->mutex);
                break;
            case 3:
                //TODO napravit to, na presmerovanie na server a odtial stiahnut :/
                pthread_mutex_lock(d->data->mutex);
                *d->data->world = load_world("vzory.txt", 1);
                plocha_vypis(&d->data->world->plocha);
                pthread_mutex_unlock(d->data->mutex);
                break;
            case 5:
                pthread_mutex_lock(d->data->mutex);
                *d->data->koniec = T;
                pthread_mutex_unlock(d->data->mutex);
                pthread_cond_signal(d->pauza);
                break;
            default: break;
        }

        if (vyber == 1 || vyber == 2 || vyber == 6) {

            int vyber2;
            do {
                vyber2 = menu_dva();

                switch (vyber2) {
                    case 1:
                        pthread_mutex_lock(d->data->mutex);
                        *d->data->pauza = (*d->data->pauza == T ? F : T);
                        pthread_mutex_unlock(d->data->mutex);
                        pthread_cond_signal(d->pauza);
                        break;
                    case 2:
                        pthread_mutex_lock(d->data->mutex);
                        save_world("worlds.txt", 0, d->data->world);
                        pthread_mutex_unlock(d->data->mutex);
                        break;
                    case 5:
                        pthread_mutex_lock(d->data->mutex);
                        *d->data->pauza = T;
                        pthread_mutex_unlock(d->data->mutex);
                        break;
                    default:
                        break;
                }
            } while (vyber2 != 5);
        }
    } while (vyber != 5);

    return 0;
}