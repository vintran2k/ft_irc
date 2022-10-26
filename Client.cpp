#include "Client.hpp"

Client::Client(int fd, std::string const & ip) : _fd(fd), _ip(ip)
{ 

	std::cout
	<< BPURPLE << _ip //
	<< GRAY << "\tClient on socket "
	<< BPURPLE <<  "[" << _fd << "]"
	<< GREEN << " connected." << WHITE << std::endl;
}

Client::~Client() {

	close(_fd);
	std::cout
	<< BPURPLE << _ip //
	<< GRAY << "\tClient on socket "
	<< BPURPLE <<  "[" << _fd << "]"
	<< RED << " disconnected." << WHITE << std::endl;
}

int						Client::getFd() const { return _fd; }

std::string const &		Client::getCmd() const { return _cmd; }

bool	Client::haveData() {

	if (_readBuffer.size())
	{
		_cmdSize = _readBuffer.find_first_of('\n');
		if (_cmdSize == std::string::npos)
		{
			_cmdSize = _readBuffer.find('\r');
			if (_cmdSize == std::string::npos)
				return false;
		}
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