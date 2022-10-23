#pragma once

#include "libs.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#define	MAX_CONNECTIONS		100

class Server {

	public:
		Server(int port, std::string const & password);
		~Server();

		int									getPort() const;
		std::string const &					getPassword() const;
		Socket const &						getSocket() const;
		std::map<int, Client *> const &		getClients() const;

		void	connectClient();
		void	deleteClient(int const fd);
		int		selectFd();
		void	run();


	private:
		int							_port;
		std::string const			_password;
		Socket						_socket;
		std::map<int, Client *>		_clients;
		fd_set						_readFds;
		int							_fdMax;

		Server();
		Server(Server const & rhs);
		Server &	operator=(Server const & rhs);
};