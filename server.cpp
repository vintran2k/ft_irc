#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

int main() {
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);     // AF_INET -> adresse IPV4, SOCK_STREAM --> TCP
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(30000);

    bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));   // bind pour connecter le server
    printf("bind: %d\n", socketServer);

    listen(socketServer, 5);    //  pour que le server puisse ecouter sur le port 30000 (defini l. 6), jusqu'a 5 connexions clients
                                //  il va attendre que les clients se connectent
    printf("\nlisten\n");

    struct sockaddr_in addrClient;
    socklen_t   csize = sizeof(addrClient);
    int         socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);    //  accept fait une boucle infini pour att que le client se connecte
                                                                                                //  une fois connecte, il creer le socket
    printf("\naccept\n");

    printf("\nclient: %d\n", socketClient);

    std::string data = "server";                     // on envois une data au client
    send(socketClient, &data, sizeof(data), 0);

    if (recv(socketClient, &data, sizeof(data), 0) > 0)     // on recoit une data du client
        std::cout << "\ndata: " << data << "\n" << std::endl;

    close(socketClient);
    close(socketServer);
    printf("close\n");

    return (0);
}