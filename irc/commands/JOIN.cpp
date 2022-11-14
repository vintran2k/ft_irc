#include "../Irc.hpp"

//	The JOIN command is used by a user to request to start listening to
//	the specific channel.

void	Irc::_JOIN(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::vector<std::string>	channels, keys;

	if (sCmd.size() < 2)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}
	split(channels, sCmd[1], ",");
	if (sCmd.size() > 2)
		split(keys, sCmd[2], ",");

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string	key = i < keys.size() ? keys[i] : "";

		if (channels[i][0] != '#' || channels[i].find_first_of(" \a,") != std::string::npos)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, channels[i])));
		else
		{
			Channel *	channel = _findChannel(channels[i]);

			if (channel)
			{
				if (user._channels.find(channel) != user._channels.end())
					return ;
				int	ret = channel->_addUser(&user, key);
				if (ret != 0)
				{
					if (ret == 1)
						serverReply.push_back(std::make_pair(user._fd, ERR_INVITEONLYCHAN(user._nickName, channel->_name)));
					else if (ret == 2)
						serverReply.push_back(std::make_pair(user._fd, ERR_BADCHANNELKEY(user._nickName, channel->_name)));
					else if (ret == 3)
						serverReply.push_back(std::make_pair(user._fd, ERR_CHANNELISFULL(user._nickName, channel->_name)));
					return ;
				}
				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " JOIN :" + channel->_name + CLRF);
			}
			else if (!channel)
			{
				channel = _addNewChannel(channels[i], &user);
				serverReply.push_back(std::make_pair(user._fd, user._prefix + " JOIN :" + channel->_name + CLRF));
			}
			user._channels.insert(channel);
			serverReply.push_back(std::make_pair(user._fd, RPL_NAMREPLY(user._nickName, channel->_name, channel->_getNamesList(&user))));
			serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFNAMES(user._nickName, channel->_name)));
			if (!channel->_topic.empty())
				serverReply.push_back(std::make_pair(user._fd, RPL_TOPIC(user._nickName, channel->_name, channel->_topic)));
		}
	}
}