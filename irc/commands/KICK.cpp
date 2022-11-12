#include "../Irc.hpp"

void	Irc::_KICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}

	std::vector<std::string>	chanNames;
	Channel *	channel;

	split(chanNames, sCmd[1], ",");
	channel = _findChannel(*chanNames.begin());
	if (!channel)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, *chanNames.begin())));
	else if (!channel->_isInChannel(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, *chanNames.begin())));
	else if (!channel->_isOperator(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, *chanNames.begin())));
	else
	{
		std::vector<std::string>	targets;
		split(targets, sCmd[2], ",");
		for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
		{
			User *	target;
			target = _findUser(*it);
			if (target)
			{
				if (!channel->_isInChannel(target))
					serverReply.push_back(std::make_pair(user._fd, ERR_USERNOTINCHANNEL(user._nickName, target->_nickName, channel->_name)));
				else
				{
					std::string	comment = sCmd.size() == 4 ? sCmd[3] : user._nickName;
					_replyToUsers(-1, channel->_users, serverReply, user._prefix + " KICK " + channel->_name + " " + target->_nickName + " " + comment + CLRF);
					_deleteUserFromChannel(target, channel);
				}
			}
		}
	}
}