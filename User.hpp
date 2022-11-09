#pragma once

#include "includes.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Channel;

class User {

	public:

		User(int fd);
		~User();

		std::string const &		getNickName() const;
		bool					isInvisible() const;


	private:

		User(User const & rhs);
		User &	operator=(User const & rhs);

		int						_fd;
		std::string				_nickName;
		std::string				_userName;
		std::string				_hostName;
		std::string				_serverName;
		std::string				_realName;
		std::string				_prefix;
		std::string				_awayMessage;

		bool					_isPassOk;
		bool					_isRegister;
		bool					_operator;
		bool					_away;
		bool					_invisible;

		std::set<Channel *>		_channels;


		friend class Irc;
};