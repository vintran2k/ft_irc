#include "../Irc.hpp"

void	Irc::_USER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 5)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isRegister == true)
		serverReply.push_back(std::make_pair(user._fd, ERR_ALREADYREGISTRED(user._nickName)));
	else
	{
		user._userName = sCmd[1];
		user._hostName = sCmd[2];
		user._serverName = sCmd[3];
		user._realName = sCmd[4];
		if (*user._realName.begin() == ':')
			user._realName.erase(user._realName.begin());

		if (user._nickName != "*" && user._isPassOk)
		{
			user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
			user._isRegister = true;
		}
	}
}