#pragma once

#include "includes.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "Irc.hpp"

class Server {

	public:

		Server(int port, std::string const & password);
		~Server();

		int									getPort() const;
		std::string const &					getPassword() const;
		Socket const &						getSocket() const;
		std::map<int, Client *> const &		getClients() const;

		void	run();


	private:

		Server();
		Server(Server const & rhs);
		Server &	operator=(Server const & rhs);
		int							_port;
		std::string const			_password;
		Socket						_socket;
		std::map<int, Client *>		_clients;
		fd_set						_readFds;
		int							_fdMin;
		int							_fdMax;
		Irc							_irc;

		void	connectClient();
		void	deleteClient(int const fd);
		int		selectFd();
};