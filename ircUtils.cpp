#include "includes.hpp"

void	splitCmd(std::vector<std::string> & sCmd, std::string cmd) {

	size_t					pos = 0;
	std::string				delimiter = " \n\r";
	std::string::iterator	it;
	
	while ((pos = cmd.find_first_of(delimiter)) != std::string::npos)
	{
		sCmd.push_back(cmd.substr(0, pos));
		for (it = cmd.begin() + pos; it != cmd.end() && delimiter.find(*it) != std::string::npos; it++)
			pos++;
		cmd.erase(0, pos);
	}
}

std::string const	getTime()
{
	time_t	t(time(NULL));
	std::string	res(ctime(&t));
	
	return (res);
}

// void	executeCmd(int clientFd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply) {

	
// }