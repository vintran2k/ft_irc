#include "Channel.hpp"

Channel::Channel(std::string const & name, User * admin) :
	_name(name),
	_inviteOnly(false),
	_topicForOp(true),
	_noExt(true),
	_limit(-1)
{
	if (admin)
	{
		_users.insert(admin);
		_operators.insert(admin);
	}
}

Channel::~Channel() {

	// _users.clear();
	// _operators.clear();
	// _invited.clear();
}

int 	Channel::_addUser(User * user, std::string const key) {

	if (_isInChannel(user))
		return 0;
	if (_inviteOnly && _invited.find(user) == _invited.end())
		return 1;
	if (!_key.empty() && _key != key)
		return 2;
	if (_limit != -1 && _users.size() >= static_cast<unsigned int>(_limit))
		return 3;
	_users.insert(user);
	_invited.erase(user);
	return 0;
}

void	Channel::_deleteUser(User * user) {

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

void			Channel::_getModes(User * user, std::string & modes, std::string & params) const {

	if (_inviteOnly)
		modes += 'i';
	if (!_key.empty())
	{
		modes += 'k';
		params += _isInChannel(user) ? _key : "<key>";
	}
	if (_limit != -1)
	{
		std::stringstream	ss;
		std::string			limit;
		ss << _limit;
		ss >> limit;

		modes += 'l';
		params += params.empty() ? limit : " " + limit;
	}
	if (_noExt)
		modes += 'n';
	if (_topicForOp)
		modes += 't';
}

bool			Channel::_modeTakesParam(char mode, char sign, std::string & err) {

	if (mode == 'k')
	{
		err = "You must specify a parameter for the key mode. Syntax: <key>.";
		return true;
	}
	if (mode == 'l' && sign == '+')
	{
		err = "You must specify a parameter for the limit mode. Syntax: <limit>.";
		return true;
	}
	if (mode == 'o')
	{
		err = "You must specify a parameter for the op mode. Syntax: <nick>.";
		return true;
	}

	return false;
}

int				Channel::_setMode(char const mode, std::string const & param, std::string &err) {

	std::string	chanModes(CHANNEL_MODES);

	if (chanModes.find(mode) == std::string::npos)
		return 1;
	
	if (mode == 'i' && !_inviteOnly)
	{
		_inviteOnly = true;
		return 0;
	}
	else if (mode == 'k' && _key.empty())
	{
		_key = param;
		return 0;
	}
	else if (mode == 'l')
	{
		int	limit = std::atoi(param.c_str());
		if (limit <= 0 || limit > 999999999 || !isdigit(param[0]))
		{
			err = "Invalid limit mode parameter. Syntax: <limit>.";
			return 2;
		}
		_limit = limit;
		return 0;
	}
	else if (mode == 'n' && !_noExt)
	{
		_noExt = true;
		return 0;
	}
	else if (mode == 'o')
	{
		User *	target = NULL;
		for (setIt(User *) it = _users.begin(); it != _users.end(); it++)
		{
			if ((*it)->getNickName() == param)
				target = *it;
		}
		if (!target)
			return 3;
		if (!_isOperator(target))
		{
			_operators.insert(target);
			return 0;
		}
	}
	else if (mode == 't' && !_topicForOp)
	{
		_topicForOp = true;
		return 0;
	}

	return -1;
}

int				Channel::_unsetMode(char const mode, std::string const & param, std::string &err) {

	std::string	chanModes(CHANNEL_MODES);

	if (chanModes.find(mode) == std::string::npos)
		return 1;
	
	if (mode == 'i' && _inviteOnly)
	{
		_inviteOnly = false;
		return 0;
	}
	else if (mode == 'k' && !_key.empty())
	{
		if (param != _key)
		{
			err = "Invalid key mode parameter. Syntax: <key>.";
			return 2;
		}
		_key.clear();
		return 0;
	}
	else if (mode == 'l' && _limit != -1)
	{
		_limit = -1;
		return 0;
	}
	else if (mode == 'n' && _noExt)
	{
		_noExt = false;
		return 0;
	}
	else if (mode == 'o')
	{
		User *	target = NULL;
		for (setIt(User *) it = _users.begin(); it != _users.end(); it++)
		{
			if ((*it)->getNickName() == param)
				target = *it;
		}
		if (!target)
			return 3;
		if (_isOperator(target))
		{
			_operators.erase(target);
			return 0;
		}
	}
	else if (mode == 't' && _topicForOp)
	{
		_topicForOp = false;
		return 0;
	}

	return -1;
}
