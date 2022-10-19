#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::Socket(int domain, int type, int protocol) : _fd(-1) {
	
	socket(domain, type, protocol);
}

Socket::~Socket() throw() {

	if (_fd != -1)	// ?
		close(_fd); // ?
}


int						Socket::getFd() const { return _fd; }

sockaddr_in	const &		Socket::getAddr() const { return _addr; }


void	Socket::socket(int domain, int type, int protocol) {

	_fd = ::socket(domain, type, protocol);
	if (_fd == -1)
		throw std::runtime_error("socket() failed");
}

void	Socket::bind(sockaddr const *addr, socklen_t len) {

	if (::bind(_fd, addr, len) == -1)
		throw std::runtime_error("bind() failed");
	std::memcpy(&_addr, addr, len);
}