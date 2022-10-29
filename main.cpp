#include "includes.hpp"
#include "Server.hpp"

Server	*IrcServer = NULL;

bool	agrsParsing(int ac, char **av, int &port, std::string &password) {

	if (ac != 3)
	{
		std::cerr << BRED << "Usage: ./ircserv <port> <password>" << WHITE << std::endl;
		return false;
	}

	port = std::atoi(av[1]);
	if (port <= 0 || port > 65535)
	{
		std::cerr	<< BRED << "Invalid port [" << port << "].\n"
					<< "Port 1 to 65535 are available" << WHITE << std::endl;
		return false;
	}

	password = std::string(av[2]);
	return true;
}

void	signalExit(int sig) {

	if (sig == SIGINT || sig == SIGQUIT)
	{
		if (IrcServer)
		{
			std::cout << "\b\b";
			delete IrcServer;
		}
		exit(0);
	}
}


int	main(int ac, char **av) {

	int				port;
	std::string		password;

	if (!agrsParsing(ac, av, port, password))
		return 1;

	signal(SIGINT, signalExit);
	signal(SIGQUIT, signalExit);

	try
	{
		IrcServer = new Server(port, password);

		IrcServer->run();
		delete IrcServer;
	}
	catch (std::exception & e) {
		std::cerr << BRED << e.what() << WHITE << std::endl;
		return 1;
	}

	return 0;
}