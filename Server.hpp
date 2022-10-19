#pragma once

#include "libs.hpp"
#include "Socket.hpp"
#define	MAX_CONNECTIONS		100

class Server {

	public:
		Server(int port, std::string const & password);
		~Server();

		int						getPort() const;
		std::string const &		getPassword() const;
		Socket const &			getSocket() const;

	private:
		int					_port;
		std::string const	_password;
		Socket				_socket;

		Server();
		Server(Server const & rhs);
		Server &	operator=(Server const & rhs);
};