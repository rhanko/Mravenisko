//
// Created by hanko on 6. 1. 2023.
//

#ifndef MRAVENISKO_CLIENT_H
#define MRAVENISKO_CLIENT_H

#include <bits/pthreadtypes.h>
#include "../../../world/world.h"

typedef struct vlakno_data {
    pthread_mutex_t *mutex;
    WORLD world;
    int co_robit; //0->posli vzor, 1-> uloz vzor
    int client_socket;
} VLAKNO_DATA;

void *client(void *data);

#endif //MRAVENISKO_CLIENT_H
