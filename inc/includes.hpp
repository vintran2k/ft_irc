#pragma once

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <fcntl.h>
#include <ctime>
#include <signal.h>
#include <fstream>
#include "colors.hpp"

#define SERVER_NAME			"ft_irc"
#define SERVER_VERSION		"v4.2"
#define	MAX_CONNECTIONS		10000
#define ADMIN_USERNAME		"admin"
#define ADMIN_PASSWORD		"adminPass"
#define CHANNEL_MODES		"iklnot"
#define USER_MODES			"io"
#define LOGPREFIX			CYAN << "[" << getTime() << "]" << "\t"

#define vectorIt(type)					std::vector<type>::iterator
#define mapIt(type1, type2)				std::map<type1, type2>::iterator
#define setIt(type)						std::set<type>::iterator
typedef std::pair<int, std::string>		t_reply;

void				getAllIrcCommands(std::set<std::string> & ircCommands);
void				split(std::vector<std::string> & res, std::string str, std::string const & delimiter);
std::string			appendParams(std::vector<std::string> const & sCmd, std::vector<std::string>::const_iterator begin);
std::string const	getTime();
bool				maskCmp(char const * mask, char const * s);
