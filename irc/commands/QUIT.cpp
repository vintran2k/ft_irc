#include "../Irc.hpp"

//	The QUIT command is used to leave the server.
//	A client session is terminated with a quit message.

void	Irc::_QUIT(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	std::string	quitMsg;

	if (sCmd.size() == 1)
		quitMsg = "Client exited";
	else
	{
		if (sCmd[1][0] == ':')
			sCmd[1].erase(sCmd[1].begin());
		quitMsg = "Quit: " + appendParams(sCmd, sCmd.begin() + 1);
	}

	std::set<User *>	users;
	for (setIt(Channel *) it = user._channels.begin(); it != user._channels.end(); it++)
		users.insert((*it)->_users.begin(), (*it)->_users.end());

	_replyToUsers(user._fd, users, serverReply, user._prefix + " QUIT :" + quitMsg + CLRF);
	serverReply.push_back(std::make_pair(user._fd, RPL_ERR(user._userName + "@" + user._hostName, quitMsg)));
	disconnectUser(user._fd);
}