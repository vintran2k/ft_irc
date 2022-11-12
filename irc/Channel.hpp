#pragma once

#include "../inc/includes.hpp"
#include "User.hpp"

#define MAX_USERS	9999

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
		User *					_admin;
		std::set<User *>		_users;
		std::set<User *>		_operators;
		std::set<User *>		_invited;
		std::string				_key;
		bool					_inviteOnly;

		int				_addUser(User * user, std::string const key);
		void			_deleteUser(User * user);
		bool			_isInChannel(User * user) const;
		bool			_isOperator(User * user) const;
		std::string		_getNamesList(User * user) const;
		int				_getVisibleValue() const;

		friend class Irc;
};