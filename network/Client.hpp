#pragma once

#include "../inc/includes.hpp"

class Client {

	public:

		Client(int fd, std::string const & ip);
		~Client();

		int						getFd() const;
		std::string const &		getCmd() const;
		std::string const &		getReadBuffer() const;

		bool					readFd();
		bool					haveData();
		void					send(std::string const & data);

		
	private:

		Client();
		Client(Client const & rhs);
		Client &	operator=(Client const & rhs);

		int	const					_fd;
		std::string	const			_ip; //
		std::string					_cmd;
		std::string					_readBuffer;
		size_t						_cmdSize;

		int						recv();
};