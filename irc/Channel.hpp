#pragma once

#include "../inc/includes.hpp"
#include "User.hpp"

#define CHANNEL_MODES	"iklot"

class User;

class Channel {

	public:

		Channel(std::string const & name, User * admin);
		~Channel();


	private:

		Channel();
		Channel(Channel const & rhs);
		Channel &	operator=(Channel const & rhs);

		std::string const		_name;
		std::string				_topic;
		std::set<User *>		_users;
		std::set<User *>		_operators;
		std::set<User *>		_invited;
		std::string				_key;
		bool					_inviteOnly;
		bool					_topicForOp;
		int						_limit;

		int				_addUser(User * user, std::string const key);
		void			_deleteUser(User * user);
		bool			_isInChannel(User * user) const;
		bool			_isOperator(User * user) const;
		std::string		_getNamesList(User * user) const;
		int				_getVisibleValue() const;
		void			_getModes(User * user, std::string & modes, std::string & params) const;
		bool			_modeTakesParam(char mode, char sign, std::string & err);
		int				_setMode(char mode, std::string const & param, std::string & err);
		int				_unsetMode(char mode, std::string const & param, std::string &err);

		friend class Irc;
};