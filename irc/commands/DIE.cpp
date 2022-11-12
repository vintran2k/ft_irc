#include "../Irc.hpp"

void	Irc::_DIE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)sCmd;
	if (!user._operator)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOPRIVILEGES(user._nickName)));
	else
	{
		// std::cout << LOGPREFIX << BIGREEN << "Server killed by " << user._nickName << WHITE << std::endl;
		_serverKilled = true;
		_killer = user._nickName;
	}
}