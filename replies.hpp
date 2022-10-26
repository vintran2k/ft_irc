#pragma once

#include "includes.hpp"

//	001
#define RPL_WELCOME(nick)				"Welcome to the ft_irc Network, " + nick

//	002
#define RPL_YOURHOST					"Your host is " + SERVER_HOSTNAME + ", running version 1.0"

//	003
#define RPL_CREATED(datetime)			"This server was created" + datetime

//	004
// #define RPL_MYINFO(nick)			


//	431
#define ERR_NONICKNAMEGIVEN				" :No nickname given"

//432
#define ERR_ERRONEUSNICKNAME(nick)		nick + " :Erroneous nickname"

//	461
#define ERR_NEEDMOREPARAMS(cmd)			cmd + " :Not enough parameters"

//	462
#define ERR_ALREADYREGISTRED			"You may not reregister"



