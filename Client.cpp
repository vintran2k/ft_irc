#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {

	std::cout
	<< GRAY << "Client on socket "
	<< BPURPLE <<  "[" << _fd << "]"
	<< GREEN << " connected." << WHITE << std::endl;
}

Client::~Client() {

	std::cout
	<< GRAY << "Client on socket "
	<< BPURPLE <<  "[" << _fd << "]"
	<< RED << " disconnected." << WHITE << std::endl;
}

int						Client::getFd() const { return _fd; }

std::string const &		Client::getCmd() const { return _cmd; }

int		Client::recv() {

	char	buffer[2048 + 1];
	int		ret;

	ret = ::recv(_fd, buffer, 2048, 0);
	if (ret == -1)
		throw std::runtime_error("recv() failed");
	else if (ret == 0)
		return (0);

	buffer[ret] = '\0';
	_cmd.clear(); //?
	_cmd += buffer;

	return ret;
}