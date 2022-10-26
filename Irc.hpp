#pragma once

#include "includes.hpp"
#include "User.hpp"
#include "Client.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void	addUser(Client & client);
		void	getResponse(std::vector<t_response> & serverResp, int fdClient, std::string cmd);
		bool	manageCommand(std::vector<std::string> & sCmd, std::string & cmd);

		
	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const				_password;
		std::vector<std::string>		_cmds;
		std::map<int, User *>			_users;
};