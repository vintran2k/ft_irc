#pragma once

#include "includes.hpp"

#define VALIDCHAR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|^_-{}[]\\"

class User {

	public:

		User();
		~User();


	private:

		User(User const & rhs);
		User &	operator=(User const & rhs);

		std::string				_nickName;
		std::string				_userName;
		std::string				_realName;
		bool					_operator;
};