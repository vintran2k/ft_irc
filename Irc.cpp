#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password), _startTime(getTime())
{

	char const *	cmds[] =
	{
		"INVITE",
		"JOIN",
		"KICK",
		"KILL",
		"LIST",
		"MODE",
		"NAMES",
		"NICK",
		"NOTICE",
		"OPER",
		"PART",
		"PASS",
		"PING",
		"PRIVMSG",
		"QUIT",
		"TOPIC",
		"USER",
		"WHO",
		NULL
	};
	for (int i = 0; cmds[i]; i++)
		_cmds.push_back(cmds[i]);
}

Irc::~Irc() {

	mapIt(int, User *)	it;

	for(it = _users.begin(); it != _users.end(); it++)
		delete it->second;
}

void	Irc::addUser(Client & client) {

	int	clientFd = client.getFd();

	_users.insert(std::make_pair(clientFd, new User()));
	if (_password.empty())
		_users[clientFd]->_isPassOk = true;
}

bool	Irc::manageCommand(std::vector<std::string> & sCmd, std::string & cmd) {

	splitCmd(sCmd, cmd);
	
	if (std::find(_cmds.begin(), _cmds.end(), *sCmd.begin()) != _cmds.end())
		return true;
	return false;
}

void	Irc::getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd) {

	std::vector<std::string>	sCmd;
	std::string					reply;

	// serverReply.clear();
	if (manageCommand(sCmd, cmd) == false)
		serverReply.push_back(std::make_pair(fdClient, std::string(WRONG_CMD(*sCmd.begin()))));
	else
	{
		std::cout << RPL_WELCOME(_users[fdClient]->_nickName) << std::endl;
		serverReply.push_back(std::make_pair(fdClient, std::string(GOOD_CMD(*sCmd.begin()))));
		if (*sCmd.begin() == "NICK")
			NICK(*_users[fdClient], sCmd, reply);
	}
}

/*void	Irc::INVITE(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::JOIN(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::KICK(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::KILL(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::LIST(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::MODE(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::NAMES(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}*/

void	Irc::NICK(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	std::string	nickName;
	std::string	validChar(VALIDCHAR);(void)user;//

	if (sCmd.size() == 1)
	{
		reply = ERR_NONICKNAMEGIVEN;
		return ;
	}
	nickName = *(sCmd.begin() + 1);
	for (std::string::iterator it = nickName.begin(); it != nickName.end(); it++)
		if (validChar.find(*it) == std::string::npos) {
			reply = ERR_ERRONEUSNICKNAME(nickName);
			return ;
		}
	if (nickName == user._nickName)
		return ;
	
	
}

/*void	Irc::NOTICE(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::OPER(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::PART(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}*/

void	Irc::PASS(User & user, std::vector<std::string> & sCmd, std::string & reply) {

	if (user._isPassOk)
		reply = ERR_ALREADYREGISTRED;
	else if (sCmd.size() == 1)
		reply = ERR_NEEDMOREPARAMS(*sCmd.begin());
	else if (*sCmd.begin() == _password)
		user._isPassOk = true;
}

/*void	Irc::PING(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::PRIVMSG(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::QUIT(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::TOPIC(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::USER(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}

void	Irc::WHO(User & user, std::vector<std::string> & sCmd, std::string & reply) {


}*/