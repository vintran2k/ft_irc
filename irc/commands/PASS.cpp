#include "../Irc.hpp"

//	The PASS command is used to set a 'connection password'.
//	The password can and MUST be set before any attempt to register
//	the connection is made.  Currently this requires that user send a
//	PASS command before sending the NICK/USER combination.

void	Irc::_PASS(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	if (sCmd.size() == 1)
		serverReply.push_back(std::make_pair(user._fd, ERR_NEEDMOREPARAMS(user._nickName, sCmd[0])));
	else if (user._isPassOk)
		serverReply.push_back(std::make_pair(user._fd, ERR_ALREADYREGISTRED(user._nickName)));
	else if (sCmd[1] == _password)
		user._isPassOk = true;
}