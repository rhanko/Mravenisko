//
// Created by hanko on 28. 12. 2022.
//

#ifndef MRAVENISKO_CLIENT_H
#define MRAVENISKO_CLIENT_H


#include "../world/world.h"

void zobrazit_vzor(char *serverName, char *portName, char *nazov_vzoru);
void ulozit_vzor(char *serverName, char *portName, WORLD *world);
int client_main(int argc, char **argv);

#endif //MRAVENISKO_CLIENT_H
