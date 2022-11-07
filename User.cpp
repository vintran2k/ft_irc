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
