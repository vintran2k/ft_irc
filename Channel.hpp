#pragma once

#include "includes.hpp"
#include "User.hpp"

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
		User *					_admin;
		bool					_inviteOnly;
		std::set<User *>		_users;
};