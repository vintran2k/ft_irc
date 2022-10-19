#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

class Socket {
	public:
		Socket();
		Socket(int family);
		virtual ~Socket() throw();

		int						getFd() const;
		sockaddr_in const &		getAddr() const;

		void	socket(int family);
		void	bind(sockaddr const *addr, socklen_t len);
	
	private:
		int				_fd;
		sockaddr_in		_addr;

		Socket(Socket const &rhs);
		Socket &	operator=(Socket const &rhs);
};