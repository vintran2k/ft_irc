#include "../Irc.hpp"

//	The list command is used to list channels and their topics.

void	Irc::_LIST(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::map<std::string, Channel *>	channels;

	if (sCmd.size() == 1)
		channels = _channels;
	else
	{
		std::vector<std::string>	chanNames;
		Channel *					channel;

		split(chanNames, sCmd[1], ",");
		for (vectorIt(std::string) it = chanNames.begin(); it != chanNames.end(); it++)
		{
			channel = _findChannel(*it);
			if (channel)
				channels[*it] = channel;
		}
	}

	for (mapIt(std::string, Channel *) it = channels.begin(); it != channels.end(); it++)
	{
		Channel *	channel = it->second;
		int			nbUsers = channel->_getVisibleValue();
		if (nbUsers)
		{
			std::stringstream	ss;
			std::string			nb;
			ss << nbUsers;
			ss >> nb;
			serverReply.push_back(std::make_pair(user._fd, RPL_LIST(user._nickName, channel->_name, nb, channel->_topic)));
		}
	}
	serverReply.push_back(std::make_pair(user._fd, RPL_LISTEND(user._nickName)));
}