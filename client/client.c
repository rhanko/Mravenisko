//
// Created by hanko on 28. 12. 2022.
//

#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "client.h"
#include "../world/vlakna/worldThreadData.h"
#include "../world/vlakna/worldUser/world_user.h"

/**
 * Procedura na zobrazenie vzoru zo servera
 * @param serverName nazov servera
 * @param portName port servera
 * @param nazov_vzoru nazov vzoru
 */
void zobrazit_vzor(char *serverName, char *portName, char *nazov_vzoru) {

    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(serverName);
    if (server == NULL) {
        printf("Server neexistuje.\n");
    } else {

        int port = atoi(portName);
        if (port <= 0) {
            printf("Port musi byt cele cislo vacsie ako 0.\n");
        } else {

            //vytvorenie socketu <sys/socket.h>
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                printf("Chyba - socket.\n");
            } else {

                //definovanie adresy servera <arpa/inet.h>
                struct sockaddr_in serverAddress;
                bzero((char *) &serverAddress, sizeof(serverAddress));
                serverAddress.sin_family = AF_INET;
                bcopy((char *) server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length);
                serverAddress.sin_port = htons(port);

                if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
                    printf("Chyba - connect.\n");
                } else {

                    //CO SA MA SPRAVIT
                    int c = 0;
                    write(sock, &c, sizeof (int));                                          //1

                    char nazov[100];
                    strcpy(nazov, nazov_vzoru);
                    write(sock, &nazov, sizeof(char)*100);                                  //2

                    int existuje;
                    read(sock, &existuje, sizeof(int));                                 //3

                    if (existuje == 0) {
                        int x;
                        read(sock, &x, sizeof(int));                                    //4

                        int y;
                        read(sock, &y, sizeof(int));                                    //5

                        WORLD world;
                        world = world_create(nazov_vzoru, x, y, 0, 0);

                        for (int i = 0; i < x; ++i) {
                            for (int j = 0; j < y; ++j) {
                                POLICKO policko;
                                read(sock, &policko, sizeof(POLICKO));                  //6
                                world.plocha.policka[i][j] = policko;
                            }
                        }

                        //zobrazi vzor
                        plocha_vypis(&world.plocha);
                    } else {
                        printf("Vzor neexistuje.\n");
                    }
                    //uzavretie socketu <unistd.h>
                    close(sock);
                    printf("Spojenie so serverom bolo ukoncene.\n");

                }
            }
        }
    }
}

/**
 * Procedura na ulozenie vzoru na server
 * @param serverName nazov servera
 * @param portName port servera
 * @param world svet na ulozenie
 */
void ulozit_vzor(char *serverName, char *portName, WORLD *world) {

    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(serverName);
    if (server == NULL) {
        printf("Server neexistuje.\n");
    } else {

        int port = atoi(portName);
        if (port <= 0) {
            printf("Port musi byt cele cislo vacsie ako 0.\n");
        } else {

            //vytvorenie socketu <sys/socket.h>
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                printf("Chyba - socket.\n");
            } else {

                //definovanie adresy servera <arpa/inet.h>
                struct sockaddr_in serverAddress;
                bzero((char *) &serverAddress, sizeof(serverAddress));
                serverAddress.sin_family = AF_INET;
                bcopy((char *) server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length);
                serverAddress.sin_port = htons(port);

                if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
                    printf("Chyba - connect.\n");
                } else {

                    //CO SA MA SPRAVIT 0-> pre nacitanie 1->ulozenie
                    int c = 1;
                    write(sock, &c, sizeof (int));                                          //1..ukladanie

                    char nazov[100];
                    strcpy(nazov, world->nazov);

                    write(sock, &nazov, sizeof(char)*100);                                  //2
                    write(sock, &world->plocha.x, sizeof(int));                             //3
                    write(sock, &world->plocha.y, sizeof(int));                             //4

                    for (int i = 0; i < world->plocha.x; ++i) {
                        for (int j = 0; j < world->plocha.y; ++j) {
                            write(sock, &world->plocha.policka[i][j], sizeof(POLICKO));     //5
                        }
                    }

                    //vrati sa 0 ked sa to ulozi, inak sa neulozil
                    int d;
                    read(sock, &d, sizeof(int));                                        //6

                    if (d == 0) {
                        printf("Vzor uspesne ulozeny.\n");
                    } else {
                        printf("Vzor nebol ulozeny.\n");
                    }

                    //uzavretie socketu <unistd.h>
                    close(sock);
                    printf("Spojenie so serverom bolo ukoncene.\n");

                }
            }
        }
    }
}

/**
 * Spustac klienta
 * @param argc pocet argumentov
 * @param argv argumenty
 * @return chyby alebo uspesne ukoncenie
 */
int client_main(int argc, char **argv) {
    if (argc < 2) {
        printError("Klienta je nutne spustit s nasledujucimi argumentmi: adresa a port.");
    }

    //vytvorenie mutexu a inicializacia
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    //inicializacia dat pre vlakna
    BOOLEAN pauza = T;
    BOOLEAN koniec = F;

    WORLD world;

    WORLD_THREAD_DATA d = {
            &world,
            &mutex,
            &pauza,
            &koniec
    };

    //podmienka pre vlakno
    pthread_cond_t pPauza;
    pthread_cond_init(&pPauza, NULL);

    WORLD_USER_DATA ud = {
            &d,
            &pPauza,
            argv[0],
            argv[1]
    };

    WORLD_PLAYER_DATA pd = {
            &d,
            &pPauza
    };

    //vytvorenie a inicializacia vlakien
    pthread_t world_user_thread, world_player_thread;
    pthread_create(&world_user_thread, NULL, &world_user, &ud);
    pthread_create(&world_player_thread, NULL, &world_player, &pd);

    pthread_join(world_user_thread, NULL);
    pthread_join(world_player_thread, NULL);

    //znicenie mutexu a podmienky
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&pPauza);
    return 0;
}

/*int client_main(int argc, char **argv) {




    //LOGIKA TU
    printf("Spojenie so serverom bolo nadviazane.\n");
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        fgets(buffer, BUFFER_LENGTH, stdin);
        char* pos = strchr(buffer, '\n');
        if (pos != NULL) {
            *pos = '\0';
        }
        //zapis dat do socketu <unistd.h>
        write(sock, buffer, strlen(buffer) + 1);
        if (strcmp(buffer, endMsg) != 0) {
            //citanie dat zo socketu <unistd.h>
            read(sock, buffer, BUFFER_LENGTH);
            printf("Server poslal nasledujuce data:\n%s\n", buffer);
        }
        else {
            koniec = 1;
        }
    }



    return (EXIT_SUCCESS);
}*/
