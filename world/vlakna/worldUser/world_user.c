//
// Created by hanko on 4. 1. 2023.
//

#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include "world_user.h"
#include "../../../client/client.h"

/**
 * Funkcia na získanie čísla od užívateľa
 * @return vráti číslo
 */
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

/**
 * Pomocná unkcia na vytvorenie noveho sveta
 * @return svet
 */
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

/**
 * Pomocná funkcia na načítanie sveta
 * @return svet
 */
WORLD load_world() {
    printf("Zadaj názov sveta, ktorý sa má načítať: \n");
    char *nazov_sveta = malloc(sizeof (char) * 100);
    gets(nazov_sveta);

    WORLD world = world_load("worlds.txt", nazov_sveta, 0);

    return world;
}

/**
 * Procedura na ulozenie sveta
 * @param world svet, ktory chceme ulozit
 */
void save_world(WORLD *world) {
    printf("Zadaj názov sveta, pre uloženie: \n");
    char *nazov_sveta = malloc(sizeof (char) * 100);
    gets(nazov_sveta);

    world->nazov = nazov_sveta;
    world_save("worlds.txt", world, 0);
}
/**
 * Pomocná funkcia pre zobrazenie menu
 * @return vráti číslo na základe výberu z menu
 */
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

/**
 * Pomocná funkcia pre zobrazenie menu 2
 * @return vráti číslo na základe výberu z menu 2
 */
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
/**
 * Vlákno, ktoré využíva klient na prácu so svetom
 * @param data data sveta
 * @return 0
 */
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
                *d->data->world = load_world();
                pthread_mutex_unlock(d->data->mutex);
                break;
            case 3:
                printf("Zadaj názov vzoru, ktorý sa má načítať: \n");
                char *nazov_sveta = malloc(sizeof (char) * 100);
                gets(nazov_sveta);

                pthread_mutex_lock(d->data->mutex);
                zobrazit_vzor(d->serverName, d->portName, nazov_sveta);
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
                        save_world(d->data->world);
                        pthread_mutex_unlock(d->data->mutex);
                        break;
                    case 3:
                        pthread_mutex_lock(d->data->mutex);
                        ulozit_vzor(d->serverName, d->portName, d->data->world);
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
