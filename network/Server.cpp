#include "Server.hpp"

Server::Server(int port, std::string const & password) :
	_port(port),
	_password(password),
	_irc(password)
{

	//	Create socket
	int	opt = 1;
#ifdef __APPLE__
	_socket.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	fcntl(_socket.getFd(), F_SETFL, O_NONBLOCK);

	if (setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt() failed");
#else
	_socket.socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (setsockopt(_socket.getFd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt() failed");
#endif

	//	Assign an address to the socket
	sockaddr_in	addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	_socket.bind((sockaddr *)&addr, sizeof(addr));

	//	Listen for connections
	_socket.listen(MAX_CONNECTIONS);

	std::cout << LOGPREFIX << BIGREEN << "Server started on port " << port << "\n" << WHITE << std::endl;
}

Server::~Server() {

	mapIt(int, Client *)	it;

	for(it = _clients.begin(); it != _clients.end(); it++)
		delete it->second;
	// _clients.clear();
	// _serverReply.clear();
	// _clientsOFF.clear();

	std::cout << LOGPREFIX << BIGREEN << "Server stopped";
	if (_irc.getServerKilled())
		std::cout << BIRED << " (killed by " << _irc.getKiller() << ")";
	std::cout << WHITE << std::endl;
}

int		Server::getPort() const { return _port; }


void	Server::connectClient() {

	sockaddr_in	addr;
	socklen_t	addrLen = 0;

	//	Connect client to server
	int	fdClient = accept(_socket.getFd(), (sockaddr *)&addr, &addrLen);
	if (fdClient == -1)
		throw std::runtime_error("accept() failed");

	fcntl(fdClient, F_SETFL, O_NONBLOCK);

	_clients.insert(std::make_pair(fdClient, new Client(fdClient)));
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

	//	Create a list with server fd and all clients fd
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

	//	Select all fd available for reading
	int	ret = select(_fdMax + 1, &_readFds, NULL, NULL, NULL);
	if (ret == -1)
		throw std::runtime_error("select() failed");
	return (ret);
}

void	Server::recvAndMakeReply(int fdsSelected) {

	//	Loop on the set of available fd
	for (int fd = _fdMin; fd <= _fdMax && fdsSelected; fd++)
	{
		if (FD_ISSET(fd, &_readFds))
		{
			//	Connect new client on server fd
			if (fd == _socket.getFd())
				connectClient();
			else
			{
				//	Read the received data on client fd
				bool	haveData = true;
				bool	isRead = false;
				bool	disconnect = false;
				while (haveData)
				{
					isRead = _clients[fd]->readFd();
					//	Get reply from Irc
					if (isRead)
						disconnect = _irc.getReply(_serverReply, fd, _clients[fd]->getCmd());
					//	If no data after recv(): client is disconnected
					else if (_clients[fd]->getReadBuffer().empty())	// sauf ctrlD // ctrlC pb
					{
						_irc.disconnectUser(fd);
						deleteClient(fd);
						break ;
					}
					haveData = _clients[fd]->haveData();
				}
				//	Check for commands QUIT, KILL, DIE
				int	fdKilled = _irc.getFdKilled();
				if (_irc.getServerKilled())
					return ;
				if (fdKilled != -1)
					_clientsOFF.push_back(fdKilled);
				if (disconnect)
					_clientsOFF.push_back(fd);
			}
			fdsSelected--;
		}
	}
}

void	Server::run() {

	int	fdsSelected;

	while (1)
	{
		//	Receive data from clients and make reply from Irc
		fdsSelected = selectFd();
		recvAndMakeReply(fdsSelected);
		if (_irc.getServerKilled())
			return ;
		

		//	Send reply to clients
		vectorIt(t_reply)	it;
		for (it = _serverReply.begin(); it != _serverReply.end(); it++)
		{
			int	fdClient = it->first;
			if (_clients.find(fdClient) != _clients.end())
			{
				// std::cout << "SEND = " << it->second << std::endl;
				_clients[fdClient]->send(it->second);
			}
		}

		//	Delete disconnected clients
		for (vectorIt(int) it = _clientsOFF.begin(); it != _clientsOFF.end(); it++)
			deleteClient(*it);

		_serverReply.clear();
		_clientsOFF.clear();
	}
}