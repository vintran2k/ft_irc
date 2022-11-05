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
		void	deleteUser(int const fd);
		bool	getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd);


	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const					_password;
		std::string							_cmdNames[18];
		void								(Irc::*_cmds[18])(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		std::map<int, User *>				_users;
		std::map<std::string, Channel *>	_channels;
		std::string const					_startTime;

		void		_initCmds();
		int			_findCommand(std::string & cmd);
		User *		_findUser(std::string const & nickname);
		Channel *	_findChannel(std::string const & name);
		void		_addNewChannel(std::string const & name, User * user);
		void		_replyToUsers(int senderFd, std::set<User *> const & users, std::vector<t_reply> & serverReply, std::string reply);

		void	_INVITE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_JOIN(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KICK(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KILL(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_LIST(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_MODE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NAMES(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NICK(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_NOTICE(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_OPER(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PART(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PASS(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PING(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_PRIVMSG(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_QUIT(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_TOPIC(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_USER(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_WHO(User & user, int fd, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
};