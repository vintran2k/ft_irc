#pragma once

#include "libs.hpp"

class Client {

	public:

		Client(int fd);
		~Client();

		int						getFd() const;
		std::string const &		getCmd() const;
		int						recv();
		bool					readFd();
		bool					haveData();
		// std::string		send();

		
	private:
		int				_fd;
		std::string 	_cmd;
		std::string		_readBuffer;
		size_t			_cmdSize;

		Client();
		Client(Client const & rhs);
		Client &	operator=(Client const & rhs);
};