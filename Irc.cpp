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

	_cmdNames[0] = "INVITE";
	_cmdNames[1] = "JOIN";
	_cmdNames[2] = "KICK";
	_cmdNames[3] = "KILL";
	_cmdNames[4] = "LIST";
	_cmdNames[5] = "MODE";
	_cmdNames[6] = "NAMES";
	_cmdNames[7] = "NICK";
	_cmdNames[8] = "NOTICE";
	_cmdNames[9] = "OPER";
	_cmdNames[10] = "PART";
	_cmdNames[11] = "PASS";
	_cmdNames[12] = "PING";
	_cmdNames[13] = "PRIVMSG";
	_cmdNames[14] = "QUIT";
	_cmdNames[15] = "TOPIC";
	_cmdNames[16] = "USER";
	_cmdNames[17] = "WHO";

	_cmds[0] = &Irc::_INVITE;
	_cmds[1] = &Irc::_JOIN;
	_cmds[2] = &Irc::_KICK;
	_cmds[3] = &Irc::_KILL;
	_cmds[4] = &Irc::_LIST;
	_cmds[5] = &Irc::_MODE;
	_cmds[6] = &Irc::_NAMES;
	_cmds[7] = &Irc::_NICK;
	_cmds[8] = &Irc::_NOTICE;
	_cmds[9] = &Irc::_OPER;
	_cmds[10] = &Irc::_PART;
	_cmds[11] = &Irc::_PASS;
	_cmds[12] = &Irc::_PING;
	_cmds[13] = &Irc::_PRIVMSG;
	_cmds[14] = &Irc::_QUIT;
	_cmds[15] = &Irc::_TOPIC;
	_cmds[16] = &Irc::_USER;
	_cmds[17] = &Irc::_WHO;
}

void	Irc::addUser(Client & client) {

	int	clientFd = client.getFd();

	_users.insert(std::make_pair(clientFd, new User()));
	if (_password.empty())
		_users[clientFd]->_isPassOk = true;
}

void	Irc::deleteUser(int const fd) {

	if (_users.find(fd) != _users.end())
	{
		// if _users[userFd]->_isRegister
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

void		Irc::_addNewChannel(std::string const & name, User * user) {

	_channels.insert(std::make_pair(name, new Channel(name, user)));
}

int		Irc::_findCommand(std::string & cmd) {

	for (int i = 0; i < 18; i++)
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
		(this->*_cmds[cmdIndex])(*user, fdClient, sCmd, serverReply);
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


void	Irc::_INVITE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_JOIN(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::vector<std::string>	channels, keys;

	if (sCmd.size() < 2)
	{
		serverReply.push_back(std::make_pair(fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	split(channels, sCmd[1], ",");
	if (sCmd.size() > 2)
		split(keys, sCmd[2], ",");

	for (size_t i = 0; i < channels.size(); i++)
	{
		bool		valid = true;
		std::string	key = i < keys.size() ? keys[i] : "";

		if (channels[i][0] != '#' || channels[i].find_first_of(" \a,") != std::string::npos)
		{
			serverReply.push_back(std::make_pair(fd, ERR_NOSUCHCHANNEL(user._nickName, channels[i])));
			valid = false;
		}
		if (valid)
		{
			Channel *	channel = _findChannel(channels[i]);

			if (channel && user._channels.find(channel) == user._channels.end())
			{
				int	ret = channel->addUser(&user, key);
				std::cout << "RET = " << ret << std::endl;
				if (ret == 1)
					serverReply.push_back(std::make_pair(fd, ERR_INVITEONLYCHAN(user._nickName, channel->_name)));
				else if (ret == 2)
					serverReply.push_back(std::make_pair(fd, ERR_BADCHANNELKEY(user._nickName, channel->_name)));
				else if (ret == 3)
					serverReply.push_back(std::make_pair(fd, ERR_CHANNELISFULL(user._nickName, channel->_name)));
				else
				{
					serverReply.push_back(std::make_pair(fd, user._prefix + " JOIN :" + channel->_name + CLRF));
					if (!channel->_topic.empty())
						serverReply.push_back(std::make_pair(fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
					
				}
			}
			else
			{
				_addNewChannel(channels[i], &user);
				serverReply.push_back(std::make_pair(fd, user._prefix + " JOIN :" + channels[i] + CLRF));
			}
		}
	}


}

void	Irc::_KICK(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_KILL(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_LIST(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_MODE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_NAMES(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_NICK(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) { //	username ne change pas dans hexchat

	std::string	nickName;
	std::string	validChar(VALIDCHAR);(void)user;//

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(fd, ERR_NONICKNAMEGIVEN(user._nickName)));
		return ;
	}
	nickName = sCmd[1];
	for (std::string::iterator it = nickName.begin(); it != nickName.end(); it++)
		if (validChar.find(*it) == std::string::npos) {
			serverReply.push_back(std::make_pair(fd, ERR_ERRONEUSNICKNAME(user._nickName, nickName)));
			return ;
		}
	if (nickName == user._nickName)
		return ;
	if (_findUser(nickName))
	{
		serverReply.push_back(std::make_pair(fd, ERR_NICKNAMEINUSE(user._nickName, nickName)));
		return ;
	}

	//if (user._isRegister == true)

	user._nickName = nickName;
	if (user._isRegister)
		serverReply.push_back(std::make_pair(fd, user._prefix + " NICK :" + nickName + CLRF));
	if (user._userName != "*" && user._isPassOk)
		user._isRegister = true;
	user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
}

void	Irc::_NOTICE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_OPER(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
		serverReply.push_back(std::make_pair(fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (sCmd[1] != ADMIN_USERNAME || sCmd[2] != ADMIN_PASSWORD)
		serverReply.push_back(std::make_pair(fd, ERR_PASSWDMISMATCH(user._nickName)));
	else
	{
		user._operator = true;
		serverReply.push_back(std::make_pair(fd, RPL_YOUREOPER(user._nickName)));
	}
}

void	Irc::_PART(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_PASS(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	// ajouter condition
	if (sCmd.size() == 1)
		serverReply.push_back(std::make_pair(fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isPassOk)
		serverReply.push_back(std::make_pair(fd, ERR_ALREADYREGISTRED(user._nickName)));
	else if (sCmd[1] == _password)
		user._isPassOk = true;
}

void	Irc::_PING(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	std::string	arg = sCmd.size() > 1 ? sCmd[1] : "";
	serverReply.push_back(std::make_pair(fd, ":" + SERVER_HOSTNAME + " PONG " + SERVER_HOSTNAME + " :" + arg + CLRF));
}

void	Irc::_PRIVMSG(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;

	if (sCmd.size() < 3)
	{
		if (sCmd.size() == 2)
			serverReply.push_back(std::make_pair(fd, ERR_NOTEXTTOSEND(user._nickName)));
		else
			serverReply.push_back(std::make_pair(fd, ERR_NORECIPIENT(user._nickName, sCmd[0])));
		return ;
	}

	std::string	target = sCmd[1];
	std::string	text = sCmd[2];
	if (target[0] == '#')
	{
		Channel	*channel = _findChannel(target);
		if (!channel || !channel->isInChannel(&user))
		{
			serverReply.push_back(std::make_pair(fd, ERR_CANNOTSENDTOCHAN(user._nickName, target)));
			return ;
		}
		else
		{
			std::cout << "PASSAGE PRIVMSG" << std::endl;
			serverReply.push_back(std::make_pair(fd, user._prefix + " PRIVMSG " + target + " " + text + CLRF));
			for (setIt(User *) it = channel->_users.begin(); it != channel->_users.end(); it++)
			{
				// besoin de send reply a tout les users de channel avec serverReply...
			}
			// serverReply.push_back(std::make_pair(fd, user._prefix + " PRIVMSG " + target + " " + text + CLRF));
		}
	}
	// else
	// {

	// }
}

void	Irc::_QUIT(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_TOPIC(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}

void	Irc::_USER(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 5)
	{
		serverReply.push_back(std::make_pair(fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	if (user._isRegister == true)
	{
		serverReply.push_back(std::make_pair(fd, ERR_ALREADYREGISTRED(user._nickName)));
		return ;
	}

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

void	Irc::_WHO(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)user;
	(void)fd;
	(void)sCmd;
	(void)serverReply;
}