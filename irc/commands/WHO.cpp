#include "../Irc.hpp"

//	The WHO command is used by a client to generate a query which returns
//	a list of information which 'matches' the <mask> parameter given by
//	the client.

void	Irc::_WHO(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::string	mask = "*";
	if (sCmd.size() > 1)
		mask = sCmd[1];
	else
		sCmd.push_back("*");
	bool		onlyOp = false;
	if (sCmd.size() > 2 && sCmd[2].compare("o") == 0)
		onlyOp = true;
	std::string	reply;
	std::string	flags;
	if (mask[0] == '#')
	{
		Channel *	channel = _findChannel(mask);
		if (!channel)
			return ;
		for (setIt(User *) it = channel->_users.begin(); it != channel->_users.end(); it++)
		{
			if (!((!channel->_isInChannel(&user) && (*it)->_invisible) || (onlyOp && !(*it)->_operator)))
			{
				flags = (*it)->_away ? "G" : "H";
				if ((*it)->_operator)
					flags += "*";
				if (channel->_isOperator(*it))
					flags += "@";
				reply += RPL_WHOREPLY(user._nickName, channel->_name, (*it)->_userName,
					(*it)->_hostName, (*it)->_serverName, (*it)->_nickName, flags, (*it)->_realName);
			}
		}
	}
	else
	{
		size_t	pos = mask.find('*');
		if (pos != std::string::npos)
		{
			size_t	len = 0;
			for (std::string::iterator it = mask.begin() + pos + 1; it != mask.end() && *it == '*'; it++)
				len++;
			mask.erase(pos, len);
		}

		for (mapIt(int, User *) it = _users.begin(); it != _users.end(); it++)
		{
			User *	u = it->second;
			bool	valid = false;

			if (onlyOp && !u->_operator)
				valid = false;
			if (!onlyOp)
			{
				if (mask == "*" || mask == "0")
					valid = true;
				else if (maskCmp(mask.c_str(), u->_hostName.c_str()) || maskCmp(mask.c_str(), u->_serverName.c_str())
						|| maskCmp(mask.c_str(), u->_realName.c_str()) || maskCmp(mask.c_str(), u->_nickName.c_str()))
					valid = true;
				if (u != &user && u->_invisible)
					valid = false;
				else
				{
					if (mask == "*" || mask == "0")
						valid = _isCommonChannel(u, &user) ? false : true;
					else
						valid = true;
				}
				if (valid)
				{
					std::string	chan = u->_channels.empty() ? "*" : (*u->_channels.rbegin())->_name;
					flags = u->_away ? "G" : "H";
					if (u->_operator)
						flags += "*";
					reply += RPL_WHOREPLY(user._nickName, chan, u->_userName,
						u->_hostName, u->_serverName, u->_nickName, flags, u->_realName);
				}
			}
		}
	}
	if (!reply.empty())
			serverReply.push_back(std::make_pair(user._fd, reply));
	serverReply.push_back(std::make_pair(user._fd, RPL_ENDOFWHO(user._nickName, sCmd[1])));
}
