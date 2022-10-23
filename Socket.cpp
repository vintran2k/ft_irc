#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::Socket(int domain, int type, int protocol) : _fd(-1) {
	
	socket(domain, type, protocol);
}

Socket::~Socket() {

	if (_fd != -1)
		close(_fd);
}


int							Socket::getFd() const { return _fd; }

sockaddr_storage const &	Socket::getAddr() const { return _addr; }

sa_family_t					Socket::getFamily() const { return _addr.ss_family; }

void	Socket::socket(int domain, int type, int protocol) {

	_fd = ::socket(domain, type, protocol);
	if (_fd == -1)
		throw std::runtime_error("socket() failed");
}

void	Socket::bind(sockaddr const *addr, socklen_t len) {

	if (::bind(_fd, addr, len) == -1)
		throw std::runtime_error("bind() failed");
	std::memcpy(&_addr, addr, len); // ?
}

void	Socket::listen(int backlog) {

	if (::listen(_fd, backlog) == -1)
		throw std::runtime_error("listen() failed");
}