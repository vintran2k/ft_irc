#pragma once

#include "../inc/includes.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "../irc/Irc.hpp"

class Server {

	public:

		Server(int port, std::string const & password);
		~Server();

		int		getPort() const;

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
		std::string					_host;
		Irc							_irc;
		std::vector<t_reply>		_serverReply;
		std::vector<int>			_clientsOFF;

		void	connectClient();
		void	deleteClient(int const fd);
		int		selectFd();
		void	recvAndMakeReply(int fdsSelected);
};