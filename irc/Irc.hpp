#pragma once

#include "../inc/includes.hpp"
#include "../network/Client.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "replies.hpp"

class Irc {

	public:

		Irc(std::string const & password);
		~Irc();

		void					addUser(Client & client);
		void					disconnectUser(int const fd);
		bool					getReply(std::vector<t_reply> & serverReply, int fdClient, std::string cmd);
		int						getFdKilled();
		bool					getServerKilled() const;
		std::string const &		getKiller() const;


	private:

		Irc();
		Irc(Irc const & rhs);
		Irc &	operator=(Irc const & rhs);

		std::string const					_password;
		std::set<std::string>				_ircCmds;
		std::string							_cmdNames[21];
		void								(Irc::*_cmds[21])(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		std::map<int, User *>				_users;
		std::map<std::string, Channel *>	_channels;
		std::string const					_startTime;
		int									_fdKilled;
		bool								_serverKilled;
		std::string							_killer;

		void		_initCmds();
		int			_findCommand(std::string & cmd) const;
		User *		_findUser(std::string const & nickname);
		Channel *	_findChannel(std::string const & name);
		Channel *	_addNewChannel(std::string const & name, User * user);
		void		_deleteUserFromChannel(User * user, Channel * channel);
		bool		_isCommonChannel(User * a, User * b) const;
		void		_sendSuccessRegistration(User & user, std::vector<t_reply> & serverReply);
		void		_replyToUsers(int senderFd, std::set<User *> const & users, std::vector<t_reply> & serverReply, std::string reply);

		void	_AWAY(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_DIE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_INVITE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_JOIN(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KICK(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_KILL(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_LIST(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_MODE(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
		void	_MOTD(User & user, std::vector<std::string> & sCmd, std::vector<t_reply> & serverReply);
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

		void	_channelMODE(User & user, std::vector<t_reply> & serverReply, std::string const & target, std::string const & modes, std::vector<std::string> const & modeParams);
		void	_userMODE(User & user, std::vector<t_reply> & serverReply, std::string const & target, std::string const & modes);
};