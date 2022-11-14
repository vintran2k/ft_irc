#include "../Irc.hpp"

//	The NOTICE command is used similarly to PRIVMSG.  The difference
//	between NOTICE and PRIVMSG is that automatic replies MUST NEVER be
//	sent in response to a NOTICE message.

void	Irc::_NOTICE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
		return ;

	std::vector<std::string>	targets;
	split(targets, sCmd[1], ",");
	std::string	text = appendParams(sCmd, sCmd.begin() + 2);
	for (vectorIt(std::string) it = targets.begin(); it != targets.end(); it++)
	{
		std::string	target = *it;
		if (target[0] == '#')
		{
			Channel	*channel = _findChannel(target);
			if (channel)
				_replyToUsers(user._fd, channel->_users, serverReply, user._prefix + " NOTICE " + target + " " + text + CLRF);
		}
		else
		{
			User *	receiver = _findUser(target);
			if (receiver)
				serverReply.push_back(std::make_pair(receiver->_fd, user._prefix + " NOTICE " + target + " " + text + CLRF));
		}
	}
}