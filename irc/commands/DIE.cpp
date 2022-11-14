#include "../Irc.hpp"

//	An operator can use the DIE command to shutdown the server.

void	Irc::_DIE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)sCmd;
	if (!user._operator)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOPRIVILEGES(user._nickName)));
	else
	{
		_serverKilled = true;
		_killer = user._nickName;
	}
}