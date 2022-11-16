#include "../Irc.hpp"

//	The KILL command is used to cause a client-server connection to be
//	closed by the server which has the actual connection.

void	Irc::_KILL(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (!user._operator)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOPRIVILEGES(user._nickName)));
	else if (sCmd.size() < 3)
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
			std::string	comment = appendParams(sCmd, sCmd.begin() + 2);
			
			serverReply.push_back(std::make_pair(target->_fd, user._prefix + " KILL " + target->_nickName + " " + comment + CLRF));
			
			std::vector<std::string>	quitCmd;
			quitCmd.push_back("QUIT");
			quitCmd.push_back(":Killed");
			_QUIT(*target, quitCmd, serverReply);
		}
	}
}