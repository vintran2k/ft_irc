#include "User.hpp"

User::User(int fd) :
	_fd(fd),
	_nickName("*"),
	_userName("*"),
	_hostName("*"),
	_realName("*"),
	_prefix(""),
	_awayMessage(""),
	_isPassOk(false),
	_isRegister(false),
	_operator(false),
	_away(false),
	_invisible(false)
{}

User::~User() {}

std::string const &		User::getNickName() const { return _nickName; }

bool					User::isInvisible() const { return _invisible; }

void					User::_getModes(std::string & modes) const {

	if (_invisible)
		modes += "i";
	if (_operator)
		modes += "o";
}

int						User::_setMode(char const mode) {

	std::string	userModes(USER_MODES);

	if (userModes.find(mode) == std::string::npos)
		return 1;
	if (mode == 'i' && !_invisible)
	{
		_invisible = true;
		return 0;
	}
	return -1;
}

int						User::_unsetMode(char const mode) {

	std::string	userModes(USER_MODES);

	if (userModes.find(mode) == std::string::npos)
		return 1;
	if (mode == 'i' && _invisible)
	{
		_invisible = false;
		return 0;
	}
	if (mode == 'o' && _operator)
	{
		_operator = false;
		return 0;
	}
	return -1;
}