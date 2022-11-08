#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password), _startTime(getTime())
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

void	Irc::disconnectUser(int const fd) {

	if (_users.find(fd) != _users.end())
	{
		if (!_users[fd]->_channels.empty())
		{
			std::set<Channel *> &	channels = _users[fd]->_channels;
			for (setIt(Channel *) it = channels.begin(); it != channels.end(); it++)
			{
				(*it)->_deleteUser(_users[fd]);
				if ((*it)->_users.empty())
				{
					_channels.erase((*it)->_name);
					delete *it;
				}
			}
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

void	Irc::_AWAY(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
	{
		user._awayMessage = "";
		user._isAway = false;
		serverReply.push_back(std::make_pair(user._fd, RPL_UNAWAY(user._nickName)));
	}
	else
	{
		user._awayMessage = appendParams(sCmd, sCmd.begin() + 1);
		user._isAway = true;
		serverReply.push_back(std::make_pair(user._fd, RPL_NOWAWAY(user._nickName)));
	}
}

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
		if (toInvite->_isAway)
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
			serverReply.push_back(std::make_pair(user._fd, RPL_NAMREPLY(user._nickName, channel->_name, channel->_getNamesList())));
			serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFNAMES(user._nickName, channel->_name)));
			if (!channel->_topic.empty())
				serverReply.push_back(std::make_pair(user._fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
		}
	}


}

void	Irc::_KICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_KILL(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_LIST(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)sCmd;
	(void)serverReply;
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
				std::string	list = channel->_getNamesList();
				
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
		//if (user._isRegister == true)

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

	(void)user;
	(void)sCmd;
	(void)serverReply;
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

	std::string	arg = sCmd.size() > 1 ? sCmd[1] : "";
	serverReply.push_back(std::make_pair(user._fd, ":" + SERVER_HOSTNAME + " PONG " + SERVER_HOSTNAME + " :" + arg + CLRF));
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
				if (receiver->_isAway)
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

		if (user._nickName != "*" && user._isPassOk)
		{
			user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
			user._isRegister = true;
		}
	}
}

void	Irc::_WHO(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)sCmd;
	(void)serverReply;
}