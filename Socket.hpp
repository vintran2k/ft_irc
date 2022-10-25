#pragma once

#include "includes.hpp"

class Socket {

	public:

		Socket();
		Socket(int domain, int type, int protocol);
		virtual ~Socket();

		int							getFd() const;
		sockaddr_storage const &	getAddr() const;
		sa_family_t					getFamily() const;	//

		void	socket(int domain, int type, int protocol);
		void	bind(sockaddr const *addr, socklen_t len);
		void	listen(int backlog);


	private:

		Socket(Socket const & rhs);
		Socket &	operator=(Socket const & rhs);

		int					_fd;
		sockaddr_storage	_addr;
};