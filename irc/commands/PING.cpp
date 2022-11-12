#include "../Irc.hpp"

void	Irc::_PING(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() > 1)
		serverReply.push_back(std::make_pair(user._fd, ":" + SERVER_HOSTNAME + " PONG " + SERVER_HOSTNAME + " " + sCmd[1] + CLRF));
}