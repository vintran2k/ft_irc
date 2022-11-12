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

#include "colors.hpp"

#define	MAX_CONNECTIONS		100
#define VALIDCHAR			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|^_-{}[]\\"


#define vectorIt(type)					std::vector<type>::iterator
#define mapIt(type1, type2)				std::map<type1, type2>::iterator
#define setIt(type)						std::set<type>::iterator

typedef std::pair<int, std::string>		t_reply;


#define LOGPREFIX						CYAN << "[" << getTime() << "]" << "\t"


#define ADMIN_USERNAME		"admin"
#define ADMIN_PASSWORD		"adminPass"

void				split(std::vector<std::string> & res, std::string str, std::string const & delimiter);
std::string			appendParams(std::vector<std::string> const & sCmd, std::vector<std::string>::const_iterator begin);
std::string const	getTime();
bool				maskCmp(std::string const mask, std::string const s);
