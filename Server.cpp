#include "Server.hpp"

Server::Server(int port, std::string const & password) : _port(port), _password(password) {

	//	Create socket
	_socket.socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);

	//	Set options for TCP server socket
	int opt = 1;
	if (setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt() failed");

	// Assign an address to the socket
	sockaddr_in	addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	_socket.bind((sockaddr *)&addr, sizeof(addr));

	//	Listen for connections
	_socket.listen(MAX_CONNECTIONS);

	std::cout
	<<	"																			" 	<< std::endl
	<<	"	                   ,----,												"	<< std::endl
	<<	"	                 ,/   .`|												"	<< std::endl
	<<	"	    ,---,.     ,`   .'  :             ,---, ,-.----.      ,----..		"	<< std::endl
	<<	"	  ,'  .' |   ;    ;     /          ,`--.' | \\    /  \\    /   /   \\	"	<< std::endl
	<<	"	,---.'   | .'___,/    ,'           |   :  : ;   :    \\  |   :     :	"	<< std::endl
	<<	"	|   |   .' |    :     |            :   |  ' |   | .\\ :  .   |  ;. /	"	<< std::endl
	<<	"	:   :  :   ;    |.';  ;            |   :  | .   : |: |  .   ; /--`		"	<< std::endl
	<<	"	:   |  |-, `----'  |  |            '   '  ; |   |  \\ :  ;   | ;		"	<< std::endl
	<<	"	|   :  ;/|     '   :  ;            |   |  | |   : .  /  |   : |			"	<< std::endl
	<<	"	|   |   .'     |   |  '            '   :  ; ;   | |  \\  .   | '___		"	<< std::endl
	<<	"	'   :  '       '   :  |       ___  |   |  ' |   | ;\\  \\ '   ; : .'|	"	<< std::endl
	<<	"	|   |  |       ;   |.'     .'  .`| '   :  | :   ' | \\.' '   | '/  :	"	<< std::endl
	<<	"	|   :  \\       '---'    .'  .'   : ;   |.'  :   : :-'   |   :    /		"	<< std::endl
	<<	"	|   | ,'             ,---, '   .'  '---'    |   |.'      \\   \\ .'		"	<< std::endl
	<<	"	`----'               ;   |  .'              `---'         `---`			"	<< std::endl
	<<	"	                     `---'												"	<< std::endl;

}

Server::~Server() {

	// delete clients
	// close _fd
}