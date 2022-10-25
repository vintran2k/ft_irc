#pragma once

#include "includes.hpp"
#include "User.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void	addUser(int fd);
		void	manageCommand(int fdClient, std::string	cmd);

		
	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const				_password;
		std::vector<std::string>		_cmds;
		std::map<int, User *>			_users;
};