#include "User.hpp"

User::User() :
	_nickName("*"),
	_userName("*"),
	_hostName("*"),
	_realName("*"),
	_prefix(""),
	_isPassOk(false),
	_isRegister(false),
	_operator(false)
{}

User::~User() {}
