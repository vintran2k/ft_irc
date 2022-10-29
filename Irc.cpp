#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password), _startTime(getTime())
{
	_initCmds();
}

Irc::~Irc() {

	mapIt(int, User *)	it;

	for(it = _users.begin(); it != _users.end(); it++)
		delete it->second;
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

User *	Irc::_findUser(std::string nickname) {

	mapIt(int, User *)	it;
	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->_nickName == nickname)
			return it->second;
	}
	return NULL;
}

int		Irc::_findCommand(std::string & cmd) {

	for (int i = 0; i < 18; i++)
		if (cmd == _cmdNames[i])
			return i;
	return -1;
}

bool	Irc::getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd) {

	std::vector<std::string>	sCmd;
	std::string					reply;
	User *						user;
	int							cmdIndex;
	bool						isRegisterBefore;

	std::cout << BCYAN << "RECV -->" << cmd << WHITE << std::endl;
	splitCmd(sCmd, cmd);
	user = _users[fdClient];
	cmdIndex = _findCommand(sCmd[0]);
	if (cmdIndex == -1)
		serverReply.push_back(std::make_pair(fdClient, ERR_UNKNOWNCOMMAND(user->_nickName, *sCmd.begin())));
	else
	{
		isRegisterBefore = user->_isRegister;
		(this->*_cmds[cmdIndex])(*user, sCmd, reply);
		if (!reply.empty())
			serverReply.push_back(std::make_pair(fdClient, reply));
		if (sCmd[0] == "QUIT" || !user->_isPassOk)
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


void	Irc::_INVITE(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_JOIN(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_KICK(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_KILL(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_LIST(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_MODE(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_NAMES(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_NICK(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	std::string	nickName;
	std::string	validChar(VALIDCHAR);(void)user;//

	for (vectorIt(std::string) it = sCmd.begin(); it != sCmd.end(); it++)
		std::cout << YELLOW << *it << WHITE << std::endl;

	if (sCmd.size() == 1)
	{
		reply = ERR_NONICKNAMEGIVEN(user._nickName);
		return ;
	}
	nickName = sCmd[1];
	for (std::string::iterator it = nickName.begin(); it != nickName.end(); it++)
		if (validChar.find(*it) == std::string::npos) {
			reply = ERR_ERRONEUSNICKNAME(user._nickName, nickName);
			return ;
		}
	if (nickName == user._nickName)
		return ;
	if (_findUser(nickName))
	{
		reply = ERR_NICKNAMEINUSE(user._nickName, nickName);
		return ;
	}

	//if (user._isRegister == true)
		//
	user._nickName = nickName;
	if (!user._userName.empty() && user._isPassOk)
		user._isRegister = true;
}

void	Irc::_NOTICE(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_OPER(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_PART(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_PASS(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	// ajouter condition
	if (sCmd.size() == 1)
		reply = ERR_NEEDMOREPARAMS(user._nickName, sCmd[0]);
	else if (user._isPassOk)
		reply = ERR_ALREADYREGISTRED(user._nickName);
	else if (sCmd[1] == _password)
		user._isPassOk = true;
}

void	Irc::_PING(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_PRIVMSG(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_QUIT(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_TOPIC(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}

void	Irc::_USER(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	if (sCmd.size() < 5)
	{
		reply = ERR_NEEDMOREPARAMS(user._nickName, sCmd[0]);
		return ;
	}
	if (user._isRegister == true)
	{
		reply = ERR_ALREADYREGISTRED(user._nickName);
		return ;
	}

	user._nickName = sCmd[1];
	user._hostName = sCmd[2];
	user._serverName = sCmd[3];
	user._realName = sCmd[4];

	if (user._nickName.size() && user._isPassOk)
		user._isRegister = true;
}

void	Irc::_WHO(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	(void)user;
	(void)sCmd;
	(void)reply;
}