#include "../Irc.hpp"

bool	getMotd(std::vector<std::string> & motd) {

	std::ifstream	motdFile("./irc/motd");

	if (!motdFile.is_open())
		return false;
	
	std::string	line;
	while (std::getline(motdFile, line))
		motd.push_back(line);
	motdFile.close();

	return true;
}

void	Irc::_MOTD(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	(void)sCmd;
	if (_motd.empty())
	{
		if (!getMotd(_motd))
		{
			serverReply.push_back(std::make_pair(user._fd, ERR_NOMOTD(user._nickName)));
			return ;
		}
	}
	std::string	reply = RPL_MOTDSTART(user._nickName);
	for (vectorIt(std::string) it = _motd.begin(); it != _motd.end(); it++)
		reply += RPL_MOTD(user._nickName, *it);
	reply += RPL_ENDOFMOTD(user._nickName);
	serverReply.push_back(std::make_pair(user._fd, reply));
}