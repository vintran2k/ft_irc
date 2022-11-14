#include "../Irc.hpp"

//	The MODE command is used to define the modes of a channel or a user.

void	Irc::_userMODE(User & user, std::vector<t_reply> & serverReply, std::string const & target, std::string const & modes) {

	if (user._nickName != target)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_USERSDONTMATCH(user._nickName)));
		return ;
	}
	if (modes.empty())
	{
		std::string	m;
		user._getModes(m);
		if (!m.empty())
		{
			m.insert(0, "+");
			serverReply.push_back(std::make_pair(user._fd, RPL_UMODEIS(user._nickName, m)));
		}
		return ;
	}
	size_t			i = 0;
	char			sign = '+';
	while (i < modes.size())
	{
		if (modes[i] == '+' || modes[i] == '-')
		{
			sign = modes[i] == '+' ? '+' : '-';
			i++;
		}
		int	ret;
		if (sign == '+')
			ret = user._setMode(modes[i]);
		else
			ret = user._unsetMode(modes[i]);

		if (ret == 1)
			serverReply.push_back(std::make_pair(user._fd, ERR_UMODEUNKNOWNFLAG(user._nickName)));
		else if (ret == 0)
		{
			std::string	signedMode;
			signedMode.insert(0, 1, sign);
			signedMode.insert(1, 1, modes[i]);

			serverReply.push_back(std::make_pair(user._fd, user._prefix + " MODE " + user._nickName + " " + signedMode + CLRF));
		}
		i++;
	}
}

void	Irc::_channelMODE(User & user, std::vector<t_reply> & serverReply, std::string const & target, std::string const & modes, std::vector<std::string> const & modeParams) {

	Channel *	channel;
	channel = _findChannel(target);
	if (!channel)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NOSUCHCHANNEL(user._nickName, target)));
		return ;
	}
	if (modes.empty())
	{
		std::string	m, p;
		channel->_getModes(&user, m, p);
		if (!m.empty())
		{
			m.insert(0, "+");
			serverReply.push_back(std::make_pair(user._fd, RPL_CHANNELMODEIS(user._nickName, channel->_name, m, p)));
		}
		return ;
	}
	if (!channel->_isInChannel(&user))
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_USERNOTINCHANNEL(user._nickName, user._nickName, target)));
		return ;
	}
	if (!channel->_isOperator(&user))
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_CHANOPRIVSNEEDED(user._nickName, channel->_name)));
		return ;
	}

	size_t			i = 0;
	size_t			j = 0;
	char			sign = '+';
	while (i < modes.size())
	{
		if (modes[i] == '+' || modes[i] == '-')
		{
			sign = modes[i] == '+' ? '+' : '-';
			i++;
		}
		std::string		err, param;
		bool			takesParam = channel->_modeTakesParam(modes[i], sign, err);
		if (takesParam)
		{
			if (j < modeParams.size())
				param = modeParams[j++];
			else
				serverReply.push_back(std::make_pair(user._fd,
				ERR_INVALIDMODEPARAM(user._nickName, channel->_name, modes[i], "*", err)));
		}
		if ((takesParam && !param.empty()) || (!takesParam))
		{
			int	ret;
			if (sign == '+')
				ret = channel->_setMode(modes[i], param, err);
			else
				ret = channel->_unsetMode(modes[i], param, err);

			if (ret == 1)
				serverReply.push_back(std::make_pair(user._fd, ERR_UNKNOWNMODE(user._nickName, modes[i], channel->_name)));
			else if (ret == 2)
				serverReply.push_back(std::make_pair(user._fd,
				ERR_INVALIDMODEPARAM(user._nickName, channel->_name, modes[i], "*", err)));
			else if (ret == 3)
				serverReply.push_back(std::make_pair(user._fd, ERR_USERNOTINCHANNEL(user._nickName, param, channel->_name)));
			else if (ret == 0)
			{
				std::string	signedMode, prm;
				signedMode.insert(0, 1, sign);
				signedMode.insert(1, 1, modes[i]);
				if (!param.empty())
					prm = " :" + param;

				_replyToUsers(-1, channel->_users, serverReply, user._prefix + " MODE " + channel->_name + " " + signedMode + prm + CLRF);
			}
		}
		i++;
	}
}

void	Irc::_MODE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
		return ;
	}

	std::string const	target = sCmd[1];
	std::string const	modes = sCmd.size() > 2 ? sCmd[2] : "";

	if (target[0] == '#')
	{
		std::vector<std::string>	modeParams;
		if (sCmd.size() > 3)
			for (vectorIt(std::string) it = sCmd.begin() + 3; it != sCmd.end(); it++)
				modeParams.push_back(*it);
		_channelMODE(user, serverReply, target, modes, modeParams);
	}
	else
		_userMODE(user, serverReply, target, modes);
}