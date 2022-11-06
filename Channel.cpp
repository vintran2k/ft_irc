#include "Channel.hpp"

Channel::Channel(std::string const & name, User * admin) :
	_name(name),
	_admin(admin),
	_inviteOnly(false),
	_nbUsers(0)
{
	if (admin)
	{
		_users.insert(admin);
		_operators.insert(admin);
		_nbUsers++;
	}
}

Channel::~Channel() {}

int 	Channel::addUser(User * user, std::string const key) {  // in progress

	(void)  key;
	if (_users.find(user) != _users.end())
		return 0;
	if (_inviteOnly && _invited.find(user) == _invited.end())
		return 1;
	if (!_key.empty() && _key != key)
		return 2;
	if (_nbUsers >= MAX_USERS)
		return 3;
	_users.insert(user);
	_invited.erase(user);
	_nbUsers++;
	return 0;
}

bool	Channel::isInChannel(User * user) {

	if (_users.find(user) != _users.end())
		return true;
	return false;
}

bool		Channel::isOperator(User * user) {

	if (_operators.find(user) != _operators.end())
		return true;
	return false;
}
