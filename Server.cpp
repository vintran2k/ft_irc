#include "Server.hpp"

Server::Server(int port, std::string const & password) : _port(port), _password(password) {

	//	Create socket
	_socket.socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);

	//	Set options for TCP server socket
	int opt = 1;
	if (setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt() failed");

	//	Assign an address to the socket
	sockaddr_in	addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	_socket.bind((sockaddr *)&addr, sizeof(addr));

	//	Listen for connections
	_socket.listen(MAX_CONNECTIONS);

	std::cout																			<< BYELLOW
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
	<<	"	                     `---'												"	<< std::endl
	<<	BGREEN << "\n\nServer started on " << inet_ntoa(addr.sin_addr) << ":" << port << WHITE << std::endl;

}

Server::~Server() {

	// delete clients
	// close _fd
}

int									Server::getPort() const { return _port; }

std::string const &					Server::getPassword() const { return _password; }	//

Socket const &						Server::getSocket() const { return _socket; }

std::map<int, Client *> const &		Server::getClients() const { return _clients; }


void	Server::connectClient() {

	sockaddr_in	addr;
	socklen_t	addrLen = 0;

	int	fdClient = accept(_socket.getFd(), (sockaddr *)&addr, &addrLen);
	if (fdClient == -1)
		throw std::runtime_error("accept() failed");

	fcntl(fdClient, F_SETFL, O_NONBLOCK);
	_clients.insert(std::make_pair(fdClient, new Client(fdClient)));
}

void	Server::deleteClient(int const fd) {

	if (_clients.find(fd) != _clients.end())
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
}

int		Server::selectFd() {

	int	fd = _socket.getFd();

	_fdMin = fd;
	_fdMax = fd;
	FD_ZERO(&_readFds);
	FD_SET(fd, &_readFds);

	std::map<int, Client *>::iterator	it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		fd = it->first;
		FD_SET(fd, &_readFds);
		if (fd > _fdMax)
			_fdMax = fd;
		else if (fd < _fdMin)
			_fdMin = fd;
	}

	int	ret = select(_fdMax + 1, &_readFds, NULL, NULL, NULL);
	if (ret == -1)
		throw std::runtime_error("select() failed");
	return (ret);
}

void	Server::run() {

	int		fdsSelected;

	while (1)
	{
		fdsSelected = selectFd();
		for (int fd = _fdMin; fd <= _fdMax && fdsSelected; fd++)
		{
			if (FD_ISSET(fd, &_readFds))
			{
				if (fd == _socket.getFd())
					connectClient();
				else
				{
					if (_clients[fd]->recv() != 0)
						std::cout << BLUE <<  "CMD = " << _clients[fd]->getCmd() << WHITE << std::endl;
					else
						deleteClient(fd);	// ?
				}
				fdsSelected--;
			}
		}
	}
}