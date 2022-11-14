#include "Irc.hpp"

Irc::Irc(std::string const & password) :
	_password(password),
	_startTime(getTime()),
	_fdKilled(-1),
	_serverKilled(false)
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

void				Irc::_initCmds() {

	getAllIrcCommands(_ircCmds);

	_cmdNames[0] = "AWAY";
	_cmdNames[1] = "DIE";
	_cmdNames[2] = "INVITE";
	_cmdNames[3] = "JOIN";
	_cmdNames[4] = "KICK";
	_cmdNames[5] = "KILL";
	_cmdNames[6] = "LIST";
	_cmdNames[7] = "MODE";
	_cmdNames[8] = "MOTD";
	_cmdNames[9] = "NAMES";
	_cmdNames[10] = "NICK";
	_cmdNames[11] = "NOTICE";
	_cmdNames[12] = "OPER";
	_cmdNames[13] = "PART";
	_cmdNames[14] = "PASS";
	_cmdNames[15] = "PING";
	_cmdNames[16] = "PRIVMSG";
	_cmdNames[17] = "QUIT";
	_cmdNames[18] = "TOPIC";
	_cmdNames[19] = "USER";
	_cmdNames[20] = "WHO";

	_cmds[0] = &Irc::_AWAY;
	_cmds[1] = &Irc::_DIE;
	_cmds[2] = &Irc::_INVITE;
	_cmds[3] = &Irc::_JOIN;
	_cmds[4] = &Irc::_KICK;
	_cmds[5] = &Irc::_KILL;
	_cmds[6] = &Irc::_LIST;
	_cmds[7] = &Irc::_MODE;
	_cmds[8] = &Irc::_MOTD;
	_cmds[9] = &Irc::_NAMES;
	_cmds[10] = &Irc::_NICK;
	_cmds[11] = &Irc::_NOTICE;
	_cmds[12] = &Irc::_OPER;
	_cmds[13] = &Irc::_PART;
	_cmds[14] = &Irc::_PASS;
	_cmds[15] = &Irc::_PING;
	_cmds[16] = &Irc::_PRIVMSG;
	_cmds[17] = &Irc::_QUIT;
	_cmds[18] = &Irc::_TOPIC;
	_cmds[19] = &Irc::_USER;
	_cmds[20] = &Irc::_WHO;
}

void				Irc::addUser(Client & client) {

	int	clientFd = client.getFd();

	_users.insert(std::make_pair(clientFd, new User(clientFd)));
	if (_password.empty())
		_users[clientFd]->_isPassOk = true;
}

void				Irc::_deleteUserFromChannel(User *user, Channel *channel)
{
	user->_channels.erase(channel);
	channel->_deleteUser(user);
	if (channel->_users.empty())
	{
		_channels.erase(channel->_name);
		delete channel;
	}
}

void				Irc::disconnectUser(int const fd) {

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

User *					Irc::_findUser(std::string const & nickname) {

	mapIt(int, User *)	it;
	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->_nickName == nickname)
			return it->second;
	}
	return NULL;
}

Channel *				Irc::_findChannel(std::string const & name) {

	mapIt(std::string, Channel *)	it;

	it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	else
		return NULL;
}

Channel *				Irc::_addNewChannel(std::string const & name, User * user) {

	Channel *	channel = new Channel(name, user);
	_channels.insert(std::make_pair(name, channel));
	return channel;
}

int						Irc::_findCommand(std::string & cmd) const {

	for (int i = 0; i < 21; i++)
		if (cmd == _cmdNames[i])
			return i;
	return -1;
}

bool					Irc::_isCommonChannel(User * a, User * b) const {

	for (setIt(Channel *) it = a->_channels.begin(); it != a->_channels.end(); it ++)
		if (b->_channels.find(*it) != b->_channels.end())
			return true;
	return false;
}

int						Irc::getFdKilled() {

	int	ret = _fdKilled;
	_fdKilled = -1;
	return ret;
}

bool					Irc::getServerKilled() const { return _serverKilled; }

std::string const &		Irc::getKiller() const { return _killer; }

void		Irc::_sendSuccessRegistration(User & user, std::vector<t_reply> & serverReply) {

	std::vector<std::string> sCmd;
	serverReply.push_back(std::make_pair(user._fd, RPL_WELCOME(user._nickName, user._userName, user._hostName)));
	serverReply.push_back(std::make_pair(user._fd, RPL_YOURHOST(user._nickName)));
	serverReply.push_back(std::make_pair(user._fd, RPL_CREATED(user._nickName, _startTime)));
	serverReply.push_back(std::make_pair(user._fd, RPL_MYINFO(user._nickName)));
	_MOTD(user, sCmd, serverReply);
}

bool	Irc::getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd) {

	std::vector<std::string>	sCmd;
	User *						user;
	int							cmdIndex;
	bool						isRegisterBefore;

	// std::cout << BCYAN << "RECV -->" << cmd << WHITE << std::endl;
	split(sCmd, cmd, " \n\r");
	if (sCmd[0].empty())
		return false;
	std::transform(sCmd[0].begin(), sCmd[0].end(), sCmd[0].begin(), ::toupper);
	user = _users[fdClient];
	cmdIndex = _findCommand(sCmd[0]);
	if (_ircCmds.find(sCmd[0]) == _ircCmds.end())
		serverReply.push_back(std::make_pair(fdClient, ERR_UNKNOWNCOMMAND(user->_nickName, *sCmd.begin())));
	else if (cmdIndex != -1)
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
			_sendSuccessRegistration(*user, serverReply);
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
