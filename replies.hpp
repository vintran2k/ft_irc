#pragma once

#include "includes.hpp"

#define CLRF							"\r\n"
#define SERVER_HOSTNAME					std::string("ft_irc.fr")
#define RPL_PREFIX(code, nick)			std::string(":" + SERVER_HOSTNAME + " " + code + " " + nick)


//	001
#define RPL_WELCOME(nick, user, host)			RPL_PREFIX("001", nick) + " :Welcome to the ft_irc Network, " + nick + "!" + user + "@" + host + CLRF

//	002
#define RPL_YOURHOST(nick)						RPL_PREFIX("002", nick) + " :Your host is " + SERVER_HOSTNAME + ", running version v4.2" + CLRF

//	003
#define RPL_CREATED(nick, datetime)				RPL_PREFIX("003", nick) + " :This server was created " + datetime + CLRF

//	004
// #define RPL_MYINFO(nick)			

//	301
#define RPL_AWAY(nick, senderNick, msg)			RPL_PREFIX("301", nick) + " " + senderNick + " " + msg + CLRF

//	305
#define RPL_UNAWAY(nick)						RPL_PREFIX("305", nick) + " :You are no longer marked as being away" + CLRF

//	306
#define RPL_NOWAWAY(nick)						RPL_PREFIX("306", nick) + " :You have been marked as being away" + CLRF

//	332
#define RPL_TOPIC(nick, chan, topic)			RPL_PREFIX("332", nick) + " :" + chan + " :" + topic + CLRF

//	341
#define RPL_INVITING(nick, invited, chan)		RPL_PREFIX("341", nick) + " :" + invited + " " + chan + CLRF

//  381
#define RPL_YOUREOPER(nick)						RPL_PREFIX("381", nick) + " :You are now an IRC operator" + CLRF

//	401
#define ERR_NOSUCHNICK(nick, errNick)			RPL_PREFIX("401", nick) + " :" + errNick + " :No such nick" + CLRF

//	403
#define ERR_NOSUCHCHANNEL(nick, chan)			RPL_PREFIX("403", nick) + " :" + chan + " :No such channel" + CLRF

//	404
#define ERR_CANNOTSENDTOCHAN(nick, chan)		RPL_PREFIX("404", nick) + " :" + chan + " :Cannot send to channel" + CLRF

//	411
#define ERR_NORECIPIENT(nick, command)			RPL_PREFIX("411", nick) + " :No recipient given (" + command + ")" + CLRF 

//	412
#define ERR_NOTEXTTOSEND(nick)					RPL_PREFIX("412", nick) + " :No text to send" + CLRF

//	421
#define ERR_UNKNOWNCOMMAND(nick, command)		RPL_PREFIX("421", nick) + " :" + command + " :Unknown command" + CLRF

//	431
#define ERR_NONICKNAMEGIVEN(nick)				RPL_PREFIX("431", nick) + " :No nickname given" + CLRF

//	432
#define ERR_ERRONEUSNICKNAME(nick, errNick)		RPL_PREFIX("432", nick) + " " + errNick + " :Erroneous nickname" + CLRF

//	433
#define ERR_NICKNAMEINUSE(nick, errNick)		RPL_PREFIX("433", nick) + " " + errNick + " :Nickname is already in use" + CLRF

//	442
#define ERR_NOTONCHANNEL(nick, chan)			RPL_PREFIX("442", nick) + " :" + chan + " :You're not on that channel" + CLRF

//	443
#define ERR_USERONCHANNEL(nick, invited, chan)	RPL_PREFIX("443", nick) + " :" + invited + " " + chan + " :is already on channel" + CLRF

//	451
#define ERR_NOTREGISTERED(nick)					RPL_PREFIX("451", nick)	+ " :You have not registered" + CLRF

//	461
#define ERR_NEEDMOREPARAMS(nick, cmd)			RPL_PREFIX("461", nick) + " :" + cmd + " :Not enough parameters" + CLRF

//	462
#define ERR_ALREADYREGISTRED(nick)				RPL_PREFIX("462", nick) + " :Unauthorized command (already registered)" + CLRF

//  464
#define ERR_PASSWDMISMATCH(nick)				RPL_PREFIX("464", nick) + " :Password incorrect" + CLRF

//	471
#define ERR_CHANNELISFULL(nick, chan)			RPL_PREFIX("471", nick) + " :" + chan + " :Cannot join channel (+l)" + CLRF

//	473
#define ERR_INVITEONLYCHAN(nick, chan)			RPL_PREFIX("473", nick) + " :" + chan + " :Cannot join channel (+i)" + CLRF

//	475
#define ERR_BADCHANNELKEY(nick, chan)			RPL_PREFIX("475", nick) + " :" + chan + " :Cannot join channel (+k)" + CLRF

//	482
#define ERR_CHANOPRIVSNEEDED(nick, chan)		RPL_PREFIX("482", nick) + " :" + chan + " :You're not channel operator" + CLRF
