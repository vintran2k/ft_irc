#pragma once

#include "../inc/includes.hpp"
#include "../network/Client.hpp"
#include "Channel.hpp"

#define USER_MODES "io"

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
		std::set<Channel *>		_channels;

		bool					_isPassOk;
		bool					_isRegister;
		bool					_operator;
		bool					_away;
		bool					_invisible;

		void					_getModes(std::string & modes) const;
		int						_setMode(char const mode);
		int						_unsetMode(char const mode);

		friend class Irc;
};