#pragma once

#include "includes.hpp"
#include "User.hpp"

#define MAX_USERS	1000

class User;

class Channel {

	public:

		Channel(std::string const & name, User * admin);
		~Channel();

		int		addUser(User * user, std::string const key);

	private:

		Channel();
		Channel(Channel const & rhs);
		Channel &	operator=(Channel const & rhs);

		std::string const		_name;
		User *					_admin;
		std::set<User *>		_users;
		std::set<User *>		_invited;
		std::string				_key;
		bool					_inviteOnly;
		unsigned int			_nbUsers;
};