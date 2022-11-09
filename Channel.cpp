#include "Channel.hpp"

Channel::Channel(std::string const & name, User * admin) :
	_name(name),
	_admin(admin),
	_inviteOnly(false)
{
	if (admin)
	{
		_users.insert(admin);
		_operators.insert(admin);
	}
}

Channel::~Channel() {

	_users.clear();
	_operators.clear();
	_invited.clear();
}

int 	Channel::_addUser(User * user, std::string const key) {  // in progress

	(void)  key;
	if (_users.find(user) != _users.end())
		return 0;
	if (_inviteOnly && _invited.find(user) == _invited.end())
		return 1;
	if (!_key.empty() && _key != key)
		return 2;
	if (_users.size() >= MAX_USERS)
		return 3;
	_users.insert(user);
	_invited.erase(user);
	return 0;
}

void	Channel::_deleteUser(User * user) {

	if (_admin == user)
		_admin = NULL;
	_users.erase(user);
	_operators.erase(user);
}

bool	Channel::_isInChannel(User * user) const {

	if (_users.find(user) != _users.end())
		return true;
	return false;
}

bool		Channel::_isOperator(User * user) const {

	if (_operators.find(user) != _operators.end())
		return true;
	return false;
}

std::string		Channel::_getNamesList(User * user) const {

	std::string	list;
	setIt(User *) it;

	bool	isInChannel = _isInChannel(user);

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (isInChannel || (!isInChannel && !(*it)->isInvisible()))
		{
			if (it != _users.begin())
				list += " ";
			if (_isOperator(*it))
				list += "@";
			list += (*it)->getNickName();
		}
	}
	return list;
}

int				Channel::_getVisibleValue() const {

	int	ret = 0;

	for (setIt(User *) it = _users.begin(); it != _users.end(); it++)
		if (!(*it)->isInvisible())
			ret++;
	return ret;
}
