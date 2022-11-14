#include "../Irc.hpp"

//	The PART command causes the user sending the message to be removed
//	from the list of active members for all given channels listed in the
//	parameter string.

void	Irc::_PART(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	std::vector<std::string>	chanNames;
	std::string					msg;
	split(chanNames, sCmd[1], ",");
	if (sCmd.size() > 2)
		msg = appendParams(sCmd, sCmd.begin() + 2);

	for (vectorIt(std::string) it = chanNames.begin(); it != chanNames.end(); it++)
	{
		Channel *	channel = _findChannel(*it);
		if (!channel)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, *it)));
		else if (!channel->_isInChannel(&user))
			serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, *it)));
		else 
		{
			if (!msg.empty())
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " PART " + channel->_name + " " + msg + CLRF);
			else
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " PART " + channel->_name + CLRF);
			_deleteUserFromChannel(&user, channel);
		}
	}
}