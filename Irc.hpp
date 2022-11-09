#pragma once

#include "includes.hpp"
#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void	addUser(Client & client);
		void	disconnectUser(int const fd);
		bool	getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd);
		int		getFdKilled();


	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const					_password;
		std::string							_cmdNames[19];
		void								(Irc::*_cmds[19])(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		std::map<int, User *>				_users;
		std::map<std::string, Channel *>	_channels;
		std::string const					_startTime;
		int									_fdKilled;

		void		_initCmds();
		int			_findCommand(std::string & cmd);
		User *		_findUser(std::string const & nickname);
		Channel *	_findChannel(std::string const & name);
		Channel *	_addNewChannel(std::string const & name, User * user);
		void		_deleteUserFromChannel(User * user, Channel * channel);
		void		_replyToUsers(int senderFd, std::set<User *> const & users, std::vector<t_reply> & serverReply, std::string reply);

		void	_AWAY(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_INVITE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_JOIN(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KILL(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_LIST(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_MODE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NAMES(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NOTICE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_OPER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PART(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PASS(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PING(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PRIVMSG(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_QUIT(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_TOPIC(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_USER(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_WHO(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
};