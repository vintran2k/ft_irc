#include "Server.hpp"

Server::Server(int port, std::string const & password) :
	_port(port),
	_password(password),
	_irc(password)
{

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


	std::cout << RED << _host << WHITE << std::endl;
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

	mapIt(int, Client *)	it;

	for(it = _clients.begin(); it != _clients.end(); it++)
		delete it->second;
}

int		Server::getPort() const { return _port; }


void	Server::connectClient() {

	sockaddr_in	addr;
	// sockaddr_storage    addr;
	socklen_t	addrLen = 0;

	int	fdClient = accept(_socket.getFd(), (sockaddr *)&addr, &addrLen);
	if (fdClient == -1)
		throw std::runtime_error("accept() failed");

	fcntl(fdClient, F_SETFL, O_NONBLOCK);

	std::string	clientIp = inet_ntoa(((sockaddr_in *)&addr)->sin_addr); //
	_clients.insert(std::make_pair(fdClient, new Client(fdClient, clientIp))); //
	_irc.addUser(*_clients[fdClient]);
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

	mapIt(int, Client *)	it;
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

void	Server::recvAndMakeReply(int fdsSelected) {

	for (int fd = _fdMin; fd <= _fdMax && fdsSelected; fd++)
	{
		if (FD_ISSET(fd, &_readFds))
		{
			if (fd == _socket.getFd())
				connectClient();
			else
			{
				bool	haveData = true;
				while (haveData)
				{
					if (_clients[fd]->readFd())
					{
						// std::cout << BLUE <<  "CMD = " << _clients[fd]->getCmd() << WHITE << std::endl;
						_irc.getReply(_serverReply, fd, _clients[fd]->getCmd());
					}
					else		// sauf ctrlD
					{
						deleteClient(fd);
						break ;
					}
					haveData = _clients[fd]->haveData();
				}
			}
			fdsSelected--;
		}
	}
}

void	Server::run() {

	int	fdsSelected;

	while (1)
	{
		//	recv
		fdsSelected = selectFd();
		recvAndMakeReply(fdsSelected);
		

		//	send

		vectorIt(t_reply)	it;
		for (it = _serverReply.begin(); it != _serverReply.end(); it++)
		{
			int	fdClient = it->first;
			if (_clients.find(fdClient) != _clients.end())
			{
				std::cout << "CLIENT = " << it->first << " CMD = " << it->second << std::endl;
				_clients[fdClient]->send(":" + it->second);
			}
		}
		_serverReply.clear();
	}
}