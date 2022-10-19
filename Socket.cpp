#include "Socket.hpp"

Socket::Socket() : _fd(-1) {}

Socket::Socket(int family) : _fd(-1) { socket(family); }

Socket::~Socket() throw() {

	if (_fd != -1)	// ?
		close(_fd); // ?
}


int						Socket::getFd() const { return _fd; }

sockaddr_in	const &		Socket::getAddr() const { return _addr; }


void	Socket::socket(int family) {

	_fd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
	if (_fd == -1)
		throw std::runtime_error("socket() failed");
}

void	Socket::bind(sockaddr const *addr, socklen_t len) {

	if (::bind(_fd, addr, len) == -1)
		throw std::runtime_error("bind() failed");
	std::memcpy(&_addr, addr, len);
}