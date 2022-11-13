#include "inc/includes.hpp"
#include "network/Server.hpp"

Server	*IrcServer = NULL;

void	ircDraw() {

	std::cout << BIBLUE
	<<	"																			\n"
	<<	"	                   ,----,												\n"
	<<	"	                 ,/   .`|												\n"
	<<	"	    ,---,.     ,`   .'  :             ,---, ,-.----.      ,----..		\n"
	<<	"	  ,'  .' |   ;    ;     /          ,`--.' | \\    /  \\    /   /   \\	\n"
	<<	"	,---.'   | .'___,/    ,'           |   :  : ;   :    \\  |   :     :	\n"
	<<	"	|   |   .' |    :     |            :   |  ' |   | .\\ :  .   |  ;. /	\n"
	<<	"	:   :  :   ;    |.';  ;            |   :  | .   : |: |  .   ; /--`		\n"
	<<	"	:   |  |-, `----'  |  |            '   '  ; |   |  \\ :  ;   | ;		\n"
	<<	"	|   :  ;/|     '   :  ;            |   |  | |   : .  /  |   : |			\n"
	<<	"	|   |   .'     |   |  '            '   :  ; ;   | |  \\  .   | '___		\n"
	<<	"	'   :  '       '   :  |       ___  |   |  ' |   | ;\\  \\ '   ; : .'|	\n"
	<<	"	|   |  |       ;   |.'     .'  .`| '   :  | :   ' | \\.' '   | '/  :	\n"
	<<	"	|   :  \\       '---'    .'  .'   : ;   |.'  :   : :-'   |   :    /		\n"
	<<	"	|   | ,'             ,---, '   .'  '---'    |   |.'      \\   \\ .'		\n"
	<<	"	`----'               ;   |  .'              `---'         `---`			\n"
	<<	"	                     `---'												\n\n"
	<< WHITE << std::endl;
}

bool	agrsParsing(int ac, char **av, int &port, std::string &password) {

	if (ac != 3)
	{
		std::cerr << BRED << "Usage: ./ircserv <port> <password>" << WHITE << std::endl;
		return false;
	}

	bool	validPort = true;

	for (size_t i = 0; av[1][i]; i++)
		if (!isdigit(av[1][i]))
			validPort = false;

	port = std::atoi(av[1]);
	if (port <= 0 || port > 65535)
		validPort = false;

	if (!validPort)
	{
		std::cerr	<< BRED << "Invalid port [" << av[1] << "]"
					<< WHITE "\t(Port [1] to [65535] are available)" << std::endl;
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
	
	ircDraw();

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
		delete IrcServer;
		return 1;
	}

	return 0;
}