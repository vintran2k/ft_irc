#include "../Irc.hpp"

void	Irc::_PASS(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isPassOk)
		serverReply.push_back(std::make_pair(user._fd, ERR_ALREADYREGISTRED(user._nickName)));
	else if (sCmd[1] == _password)
		user._isPassOk = true;
}