#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

class Socket {
	public:
		Socket();
		Socket(int domain, int type, int protocol);
		virtual ~Socket() throw();

		int						getFd() const;
		sockaddr_in const &		getAddr() const;

		void	socket(int domain, int type, int protocol);
		void	bind(sockaddr const *addr, socklen_t len);
	
	private:
		int				_fd;
		sockaddr_in		_addr;

		Socket(Socket const &rhs);
		Socket &	operator=(Socket const &rhs);
};