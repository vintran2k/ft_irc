#pragma once

#include "includes.hpp"
#include "User.hpp"
#include "Client.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void	addUser(Client & client);
		void	getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd);


	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const				_password;
		std::vector<std::string>		_cmds;
		std::map<int, User *>			_users;
		std::string const				_startTime;

		bool	manageCommand(std::vector<std::string> & sCmd, std::string & cmd);

		void	INVITE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	JOIN(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	KICK(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	KILL(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	LIST(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	MODE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	NAMES(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	NICK(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	NOTICE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	OPER(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	PART(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	PASS(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	PING(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	PRIVMSG(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	QUIT(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	TOPIC(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	USER(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	WHO(User & user, std::vector<std::string> & sCmd, std::string & reply);
};