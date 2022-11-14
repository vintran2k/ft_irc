#include "../Irc.hpp"

//	PRIVMSG is used to send private messages between users, as well as to
//	send messages to channels.

void	Irc::_PRIVMSG(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		if (sCmd.size() == 2)
			serverReply.push_back(std::make_pair(user._fd, ERR_NOTEXTTOSEND(user._nickName)));
		else
			serverReply.push_back(std::make_pair(user._fd, ERR_NORECIPIENT(user._nickName, sCmd[0])));
		return ;
	}

	std::vector<std::string>	targets;
	split(targets, sCmd[1], ",");
	std::string	text = appendParams(sCmd, sCmd.begin() + 2);
	for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
	{
		std::string	target = *it;
		if (target[0] == '#')
		{
			Channel	*channel = _findChannel(target);
			if (!channel)
				serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, target)));
			else if (!channel->_isInChannel(&user) && channel->_noExt)
				serverReply.push_back(std::make_pair(user._fd, ERR_CANNOTSENDTOCHAN(user._nickName, target)));
			else
				_replyToUsers(user._fd, channel->_users, serverReply, user._prefix + " PRIVMSG " + target + " " + text + CLRF);
		}
		else
		{
			User *	receiver = _findUser(target);
			if (!receiver)
				serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHNICK(user._nickName, target)));
			else
			{
				if (receiver->_away)
					serverReply.push_back(std::make_pair(user._fd, RPL_AWAY(user._nickName, receiver->_nickName, receiver->_awayMessage)));
				serverReply.push_back(std::make_pair(receiver->_fd, user._prefix + " PRIVMSG " + target + " " + text + CLRF));
			}
		}
	}
}