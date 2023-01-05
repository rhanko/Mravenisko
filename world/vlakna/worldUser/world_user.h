//
// Created by hanko on 4. 1. 2023.
//

#ifndef MRAVENISKO_WORLD_USER_H
#define MRAVENISKO_WORLD_USER_H

#include "../worldThreadData.h"
#include "../worldPlayer/world_player.h"

typedef struct world_user_data {
    WORLD_THREAD_DATA *data;
    pthread_cond_t *pauza;
} WORLD_USER_DATA;

void *world_user(void *data);

#endif //MRAVENISKO_WORLD_USER_H
