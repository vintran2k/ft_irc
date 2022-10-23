#pragma once

#include "libs.hpp"

class Client {

	public:

		Client(int fd);
		~Client();

		int						getFd() const;
		std::string const &		getCmd() const;
		int			recv();
		// std::string		send();

		
	private:
		int				_fd;
		std::string 	_cmd;

		Client();
		Client(Client const & rhs);
		Client &	operator=(Client const & rhs);
};