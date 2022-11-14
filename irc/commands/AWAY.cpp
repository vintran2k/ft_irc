#include "../Irc.hpp"

//	With the AWAY command, clients can set an automatic reply string for
//	any PRIVMSG commands directed at them (not to a channel they are on).
//	The server sends an automatic reply to the client sending the PRIVMSG
//	command.

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