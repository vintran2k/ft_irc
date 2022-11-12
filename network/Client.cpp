#include "Client.hpp"

Client::Client(int fd, std::string const & ip) : _fd(fd), _ip(ip)
{ 

	std::cout << LOGPREFIX
	// << BPURPLE << _ip //
	<< GRAY << "Client on socket "
	<< BIPURPLE <<  "[" << _fd << "]"
	<< GREEN << " connected\n" << WHITE << std::endl;
}

Client::~Client() {

	close(_fd);
	std::cout << LOGPREFIX
	// << BPURPLE << _ip //
	<< GRAY << "Client on socket "
	<< BIPURPLE <<  "[" << _fd << "]"
	<< RED << " disconnected\n" << WHITE << std::endl;
}

int						Client::getFd() const { return _fd; }

std::string const &		Client::getCmd() const { return _cmd; }

std::string const &		Client::getReadBuffer() const { return _readBuffer; }

bool	Client::haveData() {

	if (_readBuffer.size())
	{
		_cmdSize = _readBuffer.find("\n\r");
		if (_cmdSize != std::string::npos)
		{
			_cmdSize++;
			return true;
		}
		_cmdSize = _readBuffer.find('\n');
		if (_cmdSize != std::string::npos)
			return true;
	}
	return false;
}

int		Client::recv() {

	char	buffer[2048 + 1];
	int		ret;

	ret = ::recv(_fd, buffer, 2048, 0);
	if (ret == -1)
		throw std::runtime_error("recv() failed");
	else if (ret == 0)
		return (0);

	buffer[ret] = '\0';
	_readBuffer += buffer;

	return ret;
}

bool	Client::readFd() {

	if (!haveData())
		if (!recv())
			return false;
	if (haveData())
	{
		_cmd = _readBuffer.substr(0, _cmdSize + 1);
		_readBuffer.erase(0, _cmdSize + 1);
		return true;
	}
	return false;
}

void	Client::send(std::string const & data) {

	if (::send(_fd, data.c_str(), data.size(), 0) == -1)
		throw std::runtime_error("send() failed");
}