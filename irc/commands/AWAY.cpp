#include "../Irc.hpp"

void	Irc::_AWAY(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
	{
		user._awayMessage = "";
		user._away = false;
		serverReply.push_back(std::make_pair(user._fd, RPL_UNAWAY(user._nickName)));
	}
	else
	{
		user._awayMessage = appendParams(sCmd, sCmd.begin() + 1);
		user._away = true;
		serverReply.push_back(std::make_pair(user._fd, RPL_NOWAWAY(user._nickName)));
	}
}