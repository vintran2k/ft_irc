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
#include <algorithm>
#include <fcntl.h>
#include <ctime>
#include <signal.h>

#include "replies.hpp"
#include "colors.hpp"

#define	MAX_CONNECTIONS		100
#define VALIDCHAR			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|^_-{}[]\\"

#define vectorIt(type)					std::vector<type>::iterator
#define mapIt(type1, type2)				std::map<type1, type2>::iterator

typedef std::pair<int, std::string>		t_reply;

void				splitCmd(std::vector<std::string> & sCmd, std::string cmd);
std::string const	getTime();
