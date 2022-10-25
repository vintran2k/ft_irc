#pragma once

#include "includes.hpp"

class Client {

	public:

		Client(int fd, std::string const & ip);
		~Client();

		int						getFd() const;
		std::string const &		getCmd() const;

		bool					readFd();
		bool					haveData();
		// std::string			send();

		
	private:

		Client();
		Client(Client const & rhs);
		Client &	operator=(Client const & rhs);

		int	const			_fd;
		std::string	const	_ip; //
		std::string			_cmd;
		std::string			_readBuffer;
		size_t				_cmdSize;

		int						recv();
};