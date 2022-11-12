#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password), _startTime(getTime()), _fdKilled(-1)
{
	_initCmds();
}

Irc::~Irc() {

	mapIt(int, User *)	uIt;
	mapIt(std::string, Channel *) cIt;

	for (uIt = _users.begin(); uIt != _users.end(); uIt++)
		delete uIt->second;
	for (cIt = _channels.begin(); cIt != _channels.end(); cIt++)
		delete cIt->second;
	_users.clear();
	_channels.clear();
}

void	Irc::_initCmds() {

	_cmdNames[0] = "AWAY";
	_cmdNames[1] = "INVITE";
	_cmdNames[2] = "JOIN";
	_cmdNames[3] = "KICK";
	_cmdNames[4] = "KILL";
	_cmdNames[5] = "LIST";
	_cmdNames[6] = "MODE";
	_cmdNames[7] = "NAMES";
	_cmdNames[8] = "NICK";
	_cmdNames[9] = "NOTICE";
	_cmdNames[10] = "OPER";
	_cmdNames[11] = "PART";
	_cmdNames[12] = "PASS";
	_cmdNames[13] = "PING";
	_cmdNames[14] = "PRIVMSG";
	_cmdNames[15] = "QUIT";
	_cmdNames[16] = "TOPIC";
	_cmdNames[17] = "USER";
	_cmdNames[18] = "WHO";

	_cmds[0] = &Irc::_AWAY;
	_cmds[1] = &Irc::_INVITE;
	_cmds[2] = &Irc::_JOIN;
	_cmds[3] = &Irc::_KICK;
	_cmds[4] = &Irc::_KILL;
	_cmds[5] = &Irc::_LIST;
	_cmds[6] = &Irc::_MODE;
	_cmds[7] = &Irc::_NAMES;
	_cmds[8] = &Irc::_NICK;
	_cmds[9] = &Irc::_NOTICE;
	_cmds[10] = &Irc::_OPER;
	_cmds[11] = &Irc::_PART;
	_cmds[12] = &Irc::_PASS;
	_cmds[13] = &Irc::_PING;
	_cmds[14] = &Irc::_PRIVMSG;
	_cmds[15] = &Irc::_QUIT;
	_cmds[16] = &Irc::_TOPIC;
	_cmds[17] = &Irc::_USER;
	_cmds[18] = &Irc::_WHO;
}

void	Irc::addUser(Client & client) {

	int	clientFd = client.getFd();

	_users.insert(std::make_pair(clientFd, new User(clientFd)));
	if (_password.empty())
		_users[clientFd]->_isPassOk = true;
}

void	Irc::_deleteUserFromChannel(User *user, Channel *channel)
{
	user->_channels.erase(channel);
	channel->_deleteUser(user);
	if (channel->_users.empty())
	{
		_channels.erase(channel->_name);
		delete channel;
	}
}

void	Irc::disconnectUser(int const fd) {

	if (_users.find(fd) != _users.end())
	{
		if (!_users[fd]->_channels.empty())
		{
			std::set<Channel *> &	channels = _users[fd]->_channels;
			for (setIt(Channel *) it = channels.begin(); it != channels.end(); it = channels.begin())
				_deleteUserFromChannel(_users[fd], *it);
			channels.clear();
			for (mapIt(std::string, Channel *) it = _channels.begin(); it != _channels.end(); it++)
			{
				Channel *	channel = it->second;
				if (channel->_invited.find(_users[fd]) != channel->_invited.end())
					channel->_invited.erase(_users[fd]);
			}
		}
		delete _users[fd];
		_users.erase(fd);
	}
}

User *	Irc::_findUser(std::string const & nickname) {

	mapIt(int, User *)	it;
	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->_nickName == nickname)
			return it->second;
	}
	return NULL;
}

Channel *	Irc::_findChannel(std::string const & name) {

	mapIt(std::string, Channel *)	it;

	it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	else
		return NULL;
}

Channel *	Irc::_addNewChannel(std::string const & name, User * user) {

	Channel *	channel = new Channel(name, user);
	_channels.insert(std::make_pair(name, channel));
	return channel;
}

int		Irc::_findCommand(std::string & cmd) {

	for (int i = 0; i < 19; i++)
		if (cmd == _cmdNames[i])
			return i;
	return -1;
}

bool		Irc::_isCommonChannel(User * a, User * b) const {

	for (setIt(Channel *) it = a->_channels.begin(); it != a->_channels.end(); it ++)
		if (b->_channels.find(*it) != b->_channels.end())
			return true;
	return false;
}

int		Irc::getFdKilled() {

	int	ret = _fdKilled;
	_fdKilled = -1;
	return ret;
}

bool	Irc::getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd) {

	std::vector<std::string>	sCmd;
	User *						user;
	int							cmdIndex;
	bool						isRegisterBefore;

	std::cout << BCYAN << "RECV -->" << cmd << WHITE << std::endl;
	split(sCmd, cmd, " \n\r");
	if (sCmd[0].empty())
		return false;
	user = _users[fdClient];
	cmdIndex = _findCommand(sCmd[0]);
	if (cmdIndex == -1)
		serverReply.push_back(std::make_pair(fdClient, ERR_UNKNOWNCOMMAND(user->_nickName, *sCmd.begin())));
	else
	{
		if (sCmd[0] != "PASS" && !user->_isPassOk)
			return false;
		if (sCmd[0] != "PASS" && sCmd[0] != "NICK" && sCmd[0] != "USER")
		{
			if (!user->_isRegister && user->_isPassOk)
			{
				serverReply.push_back(std::make_pair(fdClient, ERR_NOTREGISTERED(user->_nickName)));
				return false;
			}
		}
		isRegisterBefore = user->_isRegister;
		(this->*_cmds[cmdIndex])(*user, sCmd, serverReply);
		if (sCmd[0] == "QUIT")
			return true;
		if (!isRegisterBefore && user->_isRegister)
		{
			serverReply.push_back(std::make_pair(fdClient, RPL_WELCOME(user->_nickName, user->_userName, user->_hostName)));
			serverReply.push_back(std::make_pair(fdClient, RPL_YOURHOST(user->_nickName)));
			serverReply.push_back(std::make_pair(fdClient, RPL_CREATED(user->_nickName, _startTime)));
		}
	}
	return false;
}

void		Irc::_replyToUsers(int senderFd, std::set<User *> const & users, std::vector<t_reply> & serverReply, std::string reply) {

	setIt(User *)	it;

	for (it = users.begin(); it != users.end(); it++)
	{
		if (senderFd != (*it)->_fd)
			serverReply.push_back(std::make_pair((*it)->_fd, reply));
	}
}

// void	Irc::_AWAY(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

// 	if (sCmd.size() == 1)
// 	{
// 		user._awayMessage = "";
// 		user._away = false;
// 		serverReply.push_back(std::make_pair(user._fd, RPL_UNAWAY(user._nickName)));
// 	}
// 	else
// 	{
// 		user._awayMessage = appendParams(sCmd, sCmd.begin() + 1);
// 		user._away = true;
// 		serverReply.push_back(std::make_pair(user._fd, RPL_NOWAWAY(user._nickName)));
// 	}
// }

void	Irc::_INVITE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}

	User *		toInvite = _findUser(sCmd[1]);
	Channel *	channel = _findChannel(sCmd[2]);
	if (!toInvite)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHNICK(user._nickName, sCmd[1])));
	else if (!channel)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, sCmd[2])));
	else if (!channel->_isInChannel(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, sCmd[2])));
	else if (channel->_isInChannel(toInvite))
		serverReply.push_back(std::make_pair(user._fd, ERR_USERONCHANNEL(user._nickName, sCmd[1], sCmd[2])));
	else if (channel->_inviteOnly && !channel->_isOperator(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, sCmd[2])));
	else
	{
		if (toInvite->_away)
			serverReply.push_back(std::make_pair(user._fd, RPL_AWAY(user._nickName, toInvite->_nickName, toInvite->_awayMessage)));
		channel->_invited.insert(toInvite);
		serverReply.push_back(std::make_pair(user._fd, RPL_INVITING(user._nickName, sCmd[1], sCmd[2])));
		serverReply.push_back(std::make_pair(toInvite->_fd, user._prefix + " INVITE " + toInvite->_nickName + " :" + sCmd[2] + CLRF));
	}
}

void	Irc::_JOIN(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::vector<std::string>	channels, keys;

	if (sCmd.size() < 2)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	split(channels, sCmd[1], ",");
	if (sCmd.size() > 2)
		split(keys, sCmd[2], ",");

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string	key = i < keys.size() ? keys[i] : "";

		if (channels[i][0] != '#' || channels[i].find_first_of(" \a,") != std::string::npos)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, channels[i])));
		else
		{
			Channel *	channel = _findChannel(channels[i]);

			if (channel)
			{
				if (user._channels.find(channel) != user._channels.end())
					return ;
				int	ret = channel->_addUser(&user, key);
				if (ret != 0)
				{
					if (ret == 1)
						serverReply.push_back(std::make_pair(user._fd, ERR_INVITEONLYCHAN(user._nickName, channel->_name)));
					else if (ret == 2)
						serverReply.push_back(std::make_pair(user._fd, ERR_BADCHANNELKEY(user._nickName, channel->_name)));
					else if (ret == 3)
						serverReply.push_back(std::make_pair(user._fd, ERR_CHANNELISFULL(user._nickName, channel->_name)));
					return ;
				}
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " JOIN :" + channel->_name + CLRF);
			}
			else if (!channel)
			{
				channel = _addNewChannel(channels[i], &user);
				serverReply.push_back(std::make_pair(user._fd, user._prefix + " JOIN :" + channel->_name + CLRF));
			}
			user._channels.insert(channel);
			serverReply.push_back(std::make_pair(user._fd, RPL_NAMREPLY(user._nickName, channel->_name, channel->_getNamesList(&user))));
			serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFNAMES(user._nickName, channel->_name)));
			if (!channel->_topic.empty())
				serverReply.push_back(std::make_pair(user._fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
		}
	}
}

void	Irc::_KICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}

	std::vector<std::string>	chanNames;
	Channel *	channel;

	split(chanNames, sCmd[1], ",");
	channel = _findChannel(*chanNames.begin());
	if (!channel)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, *chanNames.begin())));
	else if (!channel->_isInChannel(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, *chanNames.begin())));
	else if (!channel->_isOperator(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, *chanNames.begin())));
	else
	{
		std::vector<std::string>	targets;
		split(targets, sCmd[2], ",");
		for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
		{
			User *	target;
			target = _findUser(*it);
			if (target)
			{
				if (!channel->_isInChannel(target))
					serverReply.push_back(std::make_pair(user._fd, ERR_USERNOTINCHANNEL(user._nickName, target->_nickName, channel->_name)));
				else
				{
					std::string	comment = sCmd.size() == 4 ? sCmd[3] : user._nickName;
					_replyToUsers(-1, channel->_users, serverReply, user._prefix + " KICK " + channel->_name + " " + target->_nickName + " " + comment + CLRF);
					_deleteUserFromChannel(target, channel);
				}
			}
		}
	}
}

void	Irc::_KILL(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (!user._operator)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOPRIVILEGES(user._nickName)));
	else if (sCmd.size() == 1)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (sCmd[1] == SERVER_HOSTNAME)
		serverReply.push_back(std::make_pair(user._fd, ERR_CANTKILLSERVER(user._nickName)));
	else
	{
		User *	target;

		target = _findUser(sCmd[1]);
		if (!target)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHNICK(user._nickName, sCmd[1])));
		else
		{
			_fdKilled = target->_fd;
			std::string	comment;
			if (sCmd.size() > 2)
				comment = appendParams(sCmd, sCmd.begin() + 2);
			else
				comment = "Killed by " + user._nickName;
			std::vector<std::string>	quitCmd;
			quitCmd.push_back("QUIT");
			quitCmd.push_back(comment);
			_QUIT(*target, quitCmd, serverReply);
		}
	}
}

void	Irc::_LIST(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::map<std::string, Channel *>	channels;

	if (sCmd.size() == 1)
		channels = _channels;
	else
	{
		std::vector<std::string>	chanNames;
		Channel *					channel;

		split(chanNames, sCmd[1], ",");
		for (vectorIt(std::string) it = chanNames.begin(); it != chanNames.end(); it++)
		{
			channel = _findChannel(*it);
			if (channel)
				channels[*it] = channel;
		}
	}

	for (mapIt(std::string, Channel *) it = channels.begin(); it != channels.end(); it++)
	{
		Channel *	channel = it->second;
		int			nbUsers = channel->_getVisibleValue();
		if (nbUsers)
		{
			std::stringstream	ss;
			std::string			nb;
			ss << nbUsers;
			ss >> nb;
			serverReply.push_back(std::make_pair(user._fd, RPL_LIST(user._nickName, channel->_name, nb, channel->_topic)));
		}
	}
	serverReply.push_back(std::make_pair(user._fd, RPL_LISTEND(user._nickName)));
}

void	Irc::_MODE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_NAMES(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() > 1)
	{
		std::vector<std::string>	chanNames;
		split(chanNames, sCmd[1], ",");

		for (vectorIt(std::string) vIt = chanNames.begin(); vIt != chanNames.end(); vIt++)
		{
			Channel *	channel = _findChannel(*vIt);

			if (channel)
			{
				std::string	list = channel->_getNamesList(&user);
				
				if (!list.empty())
					serverReply.push_back(std::make_pair(user._fd, RPL_NAMREPLY(user._nickName, channel->_name, list)));
			}
			serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFNAMES(user._nickName, *vIt)));
		}
	}
}

void	Irc::_NICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) { //	username ne change pas dans hexchat

	std::string	nickName;
	std::string	validChar(VALIDCHAR);(void)user;//

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NONICKNAMEGIVEN(user._nickName)));
		return ;
	}
	nickName = sCmd[1];
	for (std::string::iterator it = nickName.begin(); it != nickName.end(); it++)
		if (validChar.find(*it) == std::string::npos) {
			serverReply.push_back(std::make_pair(user._fd, ERR_ERRONEUSNICKNAME(user._nickName, nickName)));
			return ;
		}
	if (nickName == user._nickName)
		return ;
	else if (_findUser(nickName))
		serverReply.push_back(std::make_pair(user._fd, ERR_NICKNAMEINUSE(user._nickName, nickName)));
	else
	{
		user._nickName = nickName;
		if (user._isRegister)
		{
			std::set<User *>	users;
			for (setIt(Channel *) it = user._channels.begin(); it != user._channels.end(); it++)
				users.insert((*it)->_users.begin(), (*it)->_users.end());
			if (users.empty())
				users.insert(&user);
			_replyToUsers(-1, users, serverReply, user._prefix + " NICK :" + nickName + CLRF);
		}
		if (user._userName != "*" && user._isPassOk)
		{
			user._isRegister = true;
			user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
		}
	}
}

void	Irc::_NOTICE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
		return ;

	std::vector<std::string>	targets;
	split(targets, sCmd[1], ",");
	std::string	text = appendParams(sCmd, sCmd.begin() + 2);
	for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
	{
		std::string	target = *it;
		if (target[0] == '#')
		{
			Channel	*channel = _findChannel(target);
			if (channel)
				_replyToUsers(user._fd, channel->_users, serverReply, user._prefix + " NOTICE " + target + " " + text + CLRF);
		}
		else
		{
			User *	receiver = _findUser(target);
			if (receiver)
				serverReply.push_back(std::make_pair(receiver->_fd, user._prefix + " NOTICE " + target + " " + text + CLRF));
		}
	}
}

void	Irc::_OPER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (sCmd[1] != ADMIN_USERNAME || sCmd[2] != ADMIN_PASSWORD)
		serverReply.push_back(std::make_pair(user._fd, ERR_PASSWDMISMATCH(user._nickName)));
	else
	{
		user._operator = true;
		serverReply.push_back(std::make_pair(user._fd, RPL_YOUREOPER(user._nickName)));
	}
}

void	Irc::_PART(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	std::vector<std::string>	chanNames;
	std::string					msg;
	split(chanNames, sCmd[1], ",");
	if (sCmd.size() > 2)
		msg = appendParams(sCmd, sCmd.begin() + 2);

	for (vectorIt(std::string) it = chanNames.begin(); it != chanNames.end(); it++)
	{
		Channel *	channel = _findChannel(*it);
		if (!channel)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, *it)));
		else if (!channel->_isInChannel(&user))
			serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, *it)));
		else 
		{
			if (!msg.empty())
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " PART " + channel->_name + " " + msg + CLRF);
			else
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " PART " + channel->_name + CLRF);
			_deleteUserFromChannel(&user, channel);
		}
	}
}

void	Irc::_PASS(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isPassOk)
		serverReply.push_back(std::make_pair(user._fd, ERR_ALREADYREGISTRED(user._nickName)));
	else if (sCmd[1] == _password)
		user._isPassOk = true;
}

void	Irc::_PING(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() > 1)
		serverReply.push_back(std::make_pair(user._fd, ":" + SERVER_HOSTNAME + " PONG " + SERVER_HOSTNAME + " " + sCmd[1] + CLRF));
}

void	Irc::_PRIVMSG(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		if (sCmd.size() == 2)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOTEXTTOSEND(user._nickName)));
		else
			serverReply.push_back(std::make_pair(user._fd, ERR_NORECIPIENT(user._nickName, sCmd[0])));
		return ;
	}

	std::vector<std::string>	targets;
	split(targets, sCmd[1], ",");
	std::string	text = appendParams(sCmd, sCmd.begin() + 2);
	for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
	{
		std::string	target = *it;
		if (target[0] == '#')
		{
			Channel	*channel = _findChannel(target);
			if (!channel)
				serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, target)));
			else if (!channel->_isInChannel(&user))
				serverReply.push_back(std::make_pair(user._fd, ERR_CANNOTSENDTOCHAN(user._nickName, target)));
			else
				_replyToUsers(user._fd, channel->_users, serverReply, user._prefix + " PRIVMSG " + target + " " + text + CLRF);
		}
		else
		{
			User *	receiver = _findUser(target);
			if (!receiver)
				serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHNICK(user._nickName, target)));
			else
			{
				if (receiver->_away)
					serverReply.push_back(std::make_pair(user._fd, RPL_AWAY(user._nickName, receiver->_nickName, receiver->_awayMessage)));
				serverReply.push_back(std::make_pair(receiver->_fd, user._prefix + " PRIVMSG " + target + " " + text + CLRF));
			}
		}
	}
}

void	Irc::_QUIT(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::string	quitMsg;

	if (sCmd.size() == 1)
		quitMsg = "Client exited";
	else
	{
		if (sCmd[1][0] == ':')
			sCmd[1].erase(sCmd[1].begin());
		quitMsg = "Quit: " + appendParams(sCmd, sCmd.begin() + 1);
	}

	std::set<User *>	users;
	for (setIt(Channel *) it = user._channels.begin(); it != user._channels.end(); it++)
		users.insert((*it)->_users.begin(), (*it)->_users.end());

	_replyToUsers(user._fd, users, serverReply, user._prefix + " QUIT :" + quitMsg + CLRF);
	serverReply.push_back(std::make_pair(user._fd, RPL_ERR(user._userName + "@" + user._hostName, quitMsg)));
	disconnectUser(user._fd);
}

void	Irc::_TOPIC(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 2)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else
	{
		Channel *	channel = _findChannel(sCmd[1]);
		if (!channel)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, sCmd[1])));
		else if (sCmd.size() == 2)
		{
			if (channel->_topic.empty())
				serverReply.push_back(std::make_pair(user._fd, RPL_NOTOPIC(user._nickName, channel->_name)));
			else
				serverReply.push_back(std::make_pair(user._fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
		}
		else
		{
			if (!channel->_isInChannel(&user))
				serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, channel->_name)));
			else if (!channel->_isOperator(&user))
				serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, channel->_name)));
			else
			{
				if (sCmd[2][0] == ':')
					sCmd[2].erase(sCmd[2].begin());
				channel->_topic = appendParams(sCmd, sCmd.begin() + 2);
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " TOPIC " + channel->_name + " :" + channel->_topic + CLRF);
			}
		}
	}
}

void	Irc::_USER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 5)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isRegister == true)
		serverReply.push_back(std::make_pair(user._fd, ERR_ALREADYREGISTRED(user._nickName)));
	else
	{
		user._userName = sCmd[1];
		user._hostName = sCmd[2];
		user._serverName = sCmd[3];
		user._realName = sCmd[4];
		if (*user._realName.begin() == ':')
			user._realName.erase(user._realName.begin());

		if (user._nickName != "*" && user._isPassOk)
		{
			user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
			user._isRegister = true;
		}
	}
}

void	Irc::_WHO(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::string	mask = "*";
	if (sCmd.size() > 1)
		mask = sCmd[1];
	else
		sCmd.push_back("*");
	bool		onlyOp = false;
	if (sCmd.size() > 2 && sCmd[2].compare("o") == 0)
		onlyOp = true;
	std::string	reply;
	std::string	flags;
	if (mask[0] == '#')
	{
		Channel *	channel = _findChannel(mask);
		if (!channel)
			return ;
		for (setIt(User *) it = channel->_users.begin(); it != channel->_users.end(); it++)
		{
			if (!((channel->_isInChannel(&user) && (*it)->_invisible) || (onlyOp && !(*it)->_operator)))
			{
				flags = (*it)->_away ? "G" : "H";
				if ((*it)->_operator)
					flags += "*";
				if (channel->_isOperator(*it))
					flags += "@";
				reply += RPL_WHOREPLY(user._nickName, channel->_name, (*it)->_userName,
					(*it)->_hostName, (*it)->_serverName, (*it)->_nickName, flags, (*it)->_realName);
			}
		}
	}
	else
	{
		size_t	pos = mask.find('*');
		if (pos != std::string::npos)
		{
			size_t	len = 0;
			for (std::string::iterator it = mask.begin() + pos + 1; it != mask.end() && *it == '*'; it++)
				len++;
			mask.erase(pos, len);
		}

		for (mapIt(int, User *) it = _users.begin(); it != _users.end(); it++)
		{
			User *	u = it->second;
			bool	valid = false;

			if (mask == "*" || mask == "0")
				valid = true;
			else
			{
				if (maskCmp(mask, u->_hostName) || maskCmp(mask, u->_serverName)
						|| maskCmp(mask, u->_realName) || maskCmp(mask, u->_nickName))
					valid = true;
			}
			if ((u != &user && u->_invisible && !_isCommonChannel(u, &user)) || (onlyOp && !u->_operator))
				valid = false;
			if (valid)
			{
				std::string	chan = u->_channels.empty() ? "*" : (*u->_channels.rbegin())->_name;
				flags = u->_away ? "G" : "H";
				if (u->_operator)
					flags += "*";
				reply += RPL_WHOREPLY(user._nickName, chan, u->_userName,
					u->_hostName, u->_serverName, u->_nickName, flags, u->_realName);
			}
		}
	}
	if (!reply.empty())
			serverReply.push_back(std::make_pair(user._fd, reply));
	serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFWHO(user._nickName, sCmd[1])));
}