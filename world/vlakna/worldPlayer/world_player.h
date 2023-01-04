//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_WORLD_PLAYER_H
#define MRAVENISKO_WORLD_PLAYER_H

#include "../worldThreadData.h"

typedef struct world_player_data {
    WORLD_THREAD_DATA *data;
    pthread_cond_t *pauza;
    pthread_cond_t *pokracuj;
} WORLD_PLAYER_DATA;

void *world_player(void *data);
#endif //MRAVENISKO_WORLD_PLAYER_H
