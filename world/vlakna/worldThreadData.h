//
// Created by hanko on 3. 1. 2023.
//

#ifndef MRAVENISKO_WORLDTHREADDATA_H
#define MRAVENISKO_WORLDTHREADDATA_H

#include <bits/pthreadtypes.h>
#include "../world.h"
#include "../../definition.h"

typedef struct world_thread_data {
    WORLD *world;
    pthread_mutex_t *mutex;
    BOOLEAN *pauza;
    BOOLEAN *koniec;
} WORLD_THREAD_DATA;


#endif //MRAVENISKO_WORLDTHREADDATA_H
