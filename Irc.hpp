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
		std::string						_cmdNames[18];
		void							(Irc::*_cmds[18])(User & user, std::vector<std::string> & sCmd, std::string & reply);
		std::map<int, User *>			_users;
		std::string const				_startTime;

		void	_initCmds();
		int		_findCommand(std::string & cmd);
		User *	_findUser(std::string nickname);

		void	_INVITE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_JOIN(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_KICK(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_KILL(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_LIST(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_MODE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_NAMES(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_NICK(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_NOTICE(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_OPER(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_PART(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_PASS(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_PING(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_PRIVMSG(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_QUIT(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_TOPIC(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_USER(User & user, std::vector<std::string> & sCmd, std::string & reply);
		void	_WHO(User & user, std::vector<std::string> & sCmd, std::string & reply);
};