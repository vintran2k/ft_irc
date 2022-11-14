#include "../Irc.hpp"

//	The TOPIC command is used to change or view the topic of a channel.

void	Irc::_TOPIC(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 2)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else
	{
		Channel *	channel = _findChannel(sCmd[1]);
		if (!channel)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, sCmd[1])));
		else if (sCmd.size() == 2)
		{
			if (channel->_topic.empty())
				serverReply.push_back(std::make_pair(user._fd, RPL_NOTOPIC(user._nickName, channel->_name)));
			else
				serverReply.push_back(std::make_pair(user._fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
		}
		else
		{
			if (!channel->_isInChannel(&user))
				serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, channel->_name)));
			else if (channel->_topicForOp && !channel->_isOperator(&user))
				serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, channel->_name)));
			else
			{
				if (sCmd[2][0] == ':')
					sCmd[2].erase(sCmd[2].begin());
				channel->_topic = appendParams(sCmd, sCmd.begin() + 2);
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " TOPIC " + channel->_name + " :" + channel->_topic + CLRF);
			}
		}
	}
}