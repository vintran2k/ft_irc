#include "User.hpp"

User::User() :
	_nickName(""),
	_userName(""),
	_hostName(""),
	_realName(""),
	_isPassOk(false),
	_isRegister(false),
	_operator(false)
{}

User::~User() {}

void	User::_joinChannel(Channel * channel) {

	if (_channels.find(channel) != _channels.end())
		return ;
	
}
