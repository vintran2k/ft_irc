#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password)
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

void	Irc::getResponse(std::vector<t_response> & serverResp, int fdClient, std::string cmd) {

	std::vector<std::string>	sCmd;

	// serverResp.clear();
	if (manageCommand(sCmd, cmd) == false)
		serverResp.push_back(std::make_pair(fdClient, std::string(WRONG_CMD(*sCmd.begin()))));
	else
	{
		serverResp.push_back(std::make_pair(fdClient, std::string(GOOD_CMD(*sCmd.begin()))));
	}
}