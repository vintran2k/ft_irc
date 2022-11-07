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
	_isAway(false)
{}

User::~User() {}

std::string const &		User::getNickName() const { return _nickName; }

std::string const &		User::getUserName() const { return _userName; }

std::string const &		User::getHostName() const { return _hostName; }
