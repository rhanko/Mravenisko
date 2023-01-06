#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "../definition.h"
#include "server.h"
#include "vlakna/client/client.h"
/**
 * Spustac servera
 * @param argc pocet argumentov
 * @param argv argumenty
 * @return chyby alebo uspesne ukoncenie
 */
int server_main(int argc, char** argv) {
    if (argc < 1) {
        printError("Sever je nutne spustit s nasledujucimi argumentmi: port.");
    }
    int port = atoi(argv[0]);
    if (port <= 0) {
        printError("Port musi byt cele cislo vacsie ako 0.");
    }

    //vytvorenie TCP socketu <sys/socket.h>
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError("Chyba - socket.");
    }

    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu
    serverAddress.sin_port = htons(port);       //nastavenie portu

    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - bind.");
    }

    int n = 100;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_cond_t dalsi;
    pthread_cond_init(&dalsi, NULL);

    int co_robit = 0;   //0 -> posli vzor, 1 -> uloz vzor
    int client_socket = 0;

    VLAKNO_DATA vd = {
            &mutex,
            co_robit,
            client_socket
    };

    //server bude prijimat nove spojenia cez socket serverSocket <sys/socket.h>
    listen(serverSocket, n);

    int koniec = 0;
    while(!koniec) {

        //server caka na pripojenie klienta <sys/socket.h>
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

        if (clientSocket < 0) {
            printError("Chyba - accept.");
        }

        //Uspesne sa pripojenie klienta
        printf("Klient sa pripojil na server.\n");

        vd.client_socket = clientSocket;

        //vytvorit vlakno
        pthread_t klient_vlakno;
        pthread_create(&klient_vlakno, NULL, &client, &vd);

    }

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&dalsi);

    return (EXIT_SUCCESS);
}