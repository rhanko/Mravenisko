//
// Created by hanko on 28. 12. 2022.
//

#include <pthread.h>

#include "client.h"
#include "../definition.h"
#include "../world/world.h"
#include "../world/vlakna/worldThreadData.h"
#include "../world/vlakna/worldUser/world_user.h"

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
            &pPauza
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


    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(argv[0]);
    if (server == NULL) {
        printError("Server neexistuje.");
    }
    int port = atoi(argv[1]);
    if (port <= 0) {
        printError("Port musi byt cele cislo vacsie ako 0.");
    }

    //vytvorenie socketu <sys/socket.h>
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printError("Chyba - socket.");
    }

    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - connect.");
    }

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

    //uzavretie socketu <unistd.h>
    close(sock);
    printf("Spojenie so serverom bolo ukoncene.\n");

    return (EXIT_SUCCESS);
}*/
