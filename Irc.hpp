#pragma once

#include "includes.hpp"
#include "User.hpp"
#include "Client.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void	addUser(Client & client);
		void	getReply(std::vector<t_reply> & serverResp, int fdClient, std::string cmd);


	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const				_password;
		std::vector<std::string>		_cmds;
		std::map<int, User *>			_users;
		std::string const				_startTime;

		bool	manageCommand(std::vector<std::string> & sCmd, std::string & cmd);

		void	INVITE(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	JOIN(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	KICK(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	KILL(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	LIST(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	MODE(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	NAMES(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	NICK(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	NOTICE(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	OPER(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	PART(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	PASS(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	PING(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	PRIVMSG(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	QUIT(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	TOPIC(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	USER(User & user, std::vector<std::string> & sCmd, std::string & resp);
		void	WHO(User & user, std::vector<std::string> & sCmd, std::string & resp);
};