#pragma once

#include "includes.hpp"

#define CLRF							"\r\n"
#define SERVER_HOSTNAME					std::string("ft_irc.fr")
#define REPLY_PREFIX(code, nick)		std::string(":" + SERVER_HOSTNAME + " " + code + " " + nick + " ")


//	001
#define RPL_WELCOME(nick, user, host)			REPLY_PREFIX("001", nick) + "Welcome to the ft_irc Network, " + nick + "!" + user + "@" + host + CLRF

//	002
#define RPL_YOURHOST(nick)						REPLY_PREFIX("002", nick) + "Your host is " + SERVER_HOSTNAME + ", running version v4.2" + CLRF

//	003
#define RPL_CREATED(nick, datetime)				REPLY_PREFIX("003", nick) + "This server was created " + datetime + CLRF

//	004
// #define RPL_MYINFO(nick)			


//	421
#define ERR_UNKNOWNCOMMAND(nick, command)		REPLY_PREFIX("421", nick) + command + " :Unknown command" + CLRF

//	431
#define ERR_NONICKNAMEGIVEN(nick)				REPLY_PREFIX("431", nick) + ":No nickname given" + CLRF

//432
#define ERR_ERRONEUSNICKNAME(nick, errNick)		REPLY_PREFIX("432", nick) + errNick + " :Erroneous nickname" + CLRF

//	433
#define ERR_NICKNAMEINUSE(nick, errNick)		REPLY_PREFIX("433", nick) + errNick + " :Nickname is already in use" + CLRF

//	461
#define ERR_NEEDMOREPARAMS(nick, cmd)			REPLY_PREFIX("461", nick) + cmd + " :Not enough parameters" + CLRF

//	462
#define ERR_ALREADYREGISTRED(nick)				REPLY_PREFIX("462", nick) + ":Unauthorized command (already registered)" + CLRF



