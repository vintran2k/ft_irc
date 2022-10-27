#pragma once

#include "includes.hpp"
#include "Client.hpp"


class User {

	public:

		User();
		~User();


	private:

		User(User const & rhs);
		User &	operator=(User const & rhs);

		std::string				_nickName;
		std::string				_userName;
		std::string				_hostName;
		std::string				_serverName;
		std::string				_realName;

		bool					_isPassOk;
		bool					_isRegister;
		bool					_operator;

		friend class Irc;
};