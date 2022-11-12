#include "../Irc.hpp"

void	Irc::_NICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) { //	username ne change pas dans hexchat

	std::string	nickName;
	std::string	validChar(VALIDCHAR);(void)user;//

	if (sCmd.size() == 1)
	{
		serverReply.push_back(std::make_pair(user._fd, ERR_NONICKNAMEGIVEN(user._nickName)));
		return ;
	}
	nickName = sCmd[1];
	for (std::string::iterator it = nickName.begin(); it != nickName.end(); it++)
		if (validChar.find(*it) == std::string::npos) {
			serverReply.push_back(std::make_pair(user._fd, ERR_ERRONEUSNICKNAME(user._nickName, nickName)));
			return ;
		}
	if (nickName == user._nickName)
		return ;
	else if (_findUser(nickName))
		serverReply.push_back(std::make_pair(user._fd, ERR_NICKNAMEINUSE(user._nickName, nickName)));
	else
	{
		user._nickName = nickName;
		if (user._isRegister)
		{
			std::set<User *>	users;
			for (setIt(Channel *) it = user._channels.begin(); it != user._channels.end(); it++)
				users.insert((*it)->_users.begin(), (*it)->_users.end());
			if (users.empty())
				users.insert(&user);
			_replyToUsers(-1, users, serverReply, user._prefix + " NICK :" + nickName + CLRF);
		}
		if (user._userName != "*" && user._isPassOk)
		{
			user._isRegister = true;
			user._prefix = std::string(":") + user._nickName + '!' + user._userName + '@' + user._hostName;
		}
	}
}