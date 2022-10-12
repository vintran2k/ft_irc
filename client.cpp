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
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(30000);

    connect(socketClient, (const struct sockaddr *)&addrServer, sizeof(addrServer));    // connecte le client au server
    printf("connection client\n");

    std::string data; 
    if (recv(socketClient, &data, sizeof(data), 0) > 0)     // on recoit une data du server
        std::cout << "\ndata: " << data << std::endl;

    data = "iencli";
    send(socketClient, &data, sizeof(data), 0);         // on envois une data au server

    close(socketClient);
    printf("\nclose\n");

    return (0);
}