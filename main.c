#include "client/client.h"
#include "server/server.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Treba zadať aspoň jeden argument.\n");
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        server_main(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "client") == 0) {
        client_main(argc - 2, argv + 2);
    } else {
        printf("Bol zadaný zlý argument.\n");
        return 2;
    }

    return 0;
}
