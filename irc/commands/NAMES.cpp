#include "../Irc.hpp"

//	By using the NAMES command, a user can list all nicknames of users
//	in the <channel> parameter that are visible to him.

void	Irc::_NAMES(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() > 1)
	{
		std::vector<std::string>	chanNames;
		split(chanNames, sCmd[1], ",");

		for (vectorIt(std::string) it = chanNames.begin(); it != chanNames.end(); it++)
		{
			Channel *	channel = _findChannel(*it);

			if (channel)
			{
				std::string	list = channel->_getNamesList(&user);
				
				if (!list.empty())
					serverReply.push_back(std::make_pair(user._fd, RPL_NAMREPLY(user._nickName, channel->_name, list)));
			}
			serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFNAMES(user._nickName, *it)));
		}
	}
}