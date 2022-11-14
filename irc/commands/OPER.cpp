#include "../Irc.hpp"

//	A normal user uses the OPER command to obtain operator privileges.
//	The combination of <name> and <password> are REQUIRED to gain
//	Operator privileges.

void	Irc::_OPER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() < 3)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (sCmd[1] != ADMIN_USERNAME || sCmd[2] != ADMIN_PASSWORD)
		serverReply.push_back(std::make_pair(user._fd, ERR_PASSWDMISMATCH(user._nickName)));
	else
	{
		user._operator = true;
		serverReply.push_back(std::make_pair(user._fd, RPL_YOUREOPER(user._nickName)));
	}
}