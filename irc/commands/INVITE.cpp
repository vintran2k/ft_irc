#include "../Irc.hpp"

//	The INVITE command is used to invite a user to a channel.

void	Irc::_INVITE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}

	User *		toInvite = _findUser(sCmd[1]);
	Channel *	channel = _findChannel(sCmd[2]);
	if (!toInvite)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHNICK(user._nickName, sCmd[1])));
	else if (!channel)
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, sCmd[2])));
	else if (!channel->_isInChannel(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_NOTONCHANNEL(user._nickName, sCmd[2])));
	else if (channel->_isInChannel(toInvite))
		serverReply.push_back(std::make_pair(user._fd, ERR_USERONCHANNEL(user._nickName, sCmd[1], sCmd[2])));
	else if (channel->_inviteOnly && !channel->_isOperator(&user))
		serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, sCmd[2])));
	else
	{
		if (toInvite->_away)
			serverReply.push_back(std::make_pair(user._fd, RPL_AWAY(user._nickName, toInvite->_nickName, toInvite->_awayMessage)));
		channel->_invited.insert(toInvite);
		serverReply.push_back(std::make_pair(user._fd, RPL_INVITING(user._nickName, sCmd[1], sCmd[2])));
		serverReply.push_back(std::make_pair(toInvite->_fd, user._prefix + " INVITE " + toInvite->_nickName + " :" + sCmd[2] + CLRF));
	}
}