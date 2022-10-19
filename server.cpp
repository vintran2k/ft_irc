// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
#include <arpa/inet.h>

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

#include <iostream>
#include "Socket.hpp"

int main() {

	Socket socketServer;

	socketServer.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);

	socketServer.bind((sockaddr const *)&addrServer, sizeof(addrServer));
	std::cout << "bind: " << socketServer.getFd() << std::endl;

	listen(socketServer.getFd(), 5);
	std::cout << "\nlisten" << std::endl;

	struct sockaddr_in addrClient;
	socklen_t   csize = sizeof(addrClient);
	int         socketClient = accept(socketServer.getFd(), (struct sockaddr *)&addrClient, &csize);    //  accept fait une boucle infini pour att que le client se connecte
																								//  une fois connecte, il creer le socket
	std::cout << "\naccept" << std::endl;

	std::cout << "\nclient: " << socketClient << std::endl;

	int data = 1;					// on envois une data au client
	send(socketClient, &data, sizeof(data), 0);

	if (recv(socketClient, &data, sizeof(data), 0) > 0)     // on recoit une data du client
		std::cout << "\ndata: " << data << "\n" << std::endl;

	return (0);
}