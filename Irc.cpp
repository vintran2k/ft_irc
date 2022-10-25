#include "Irc.hpp"

Irc::Irc(std::string const & password) : _password(password) {

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

void	Irc::addUser(int fd) {

	_users.insert(std::make_pair(fd, new User));
}

void	Irc::manageCommand(int fdClient, std::string cmd) {

	(void)fdClient;
	std::vector<std::string>	sCmd;

	splitCmd(sCmd, cmd);
	// for (vectorIt(std::string) it = sCmd.begin(); it != sCmd.end(); it++)
		// std::cout << "|" << *it << "|" << std::endl;
	
	if (std::find(_cmds.begin(), _cmds.end(), *sCmd.begin()) != _cmds.end())
		std::cout << GREEN << "VALID CMD" << WHITE <<std::endl;

}