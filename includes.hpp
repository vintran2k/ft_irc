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

#include "colors.hpp"

#define	MAX_CONNECTIONS		100
#define HOST				ft_irc.fr

#define vectorIt(type)					std::vector<type>::iterator
#define mapIt(type1, type2)				std::map<type1, type2>::iterator

typedef std::pair<int, std::string>		t_response;

void	splitCmd(std::vector<std::string> & sCmd, std::string cmd);


// MSG
#define WRONG_CMD(cmd)					std::string(cmd + ": Unknown command")
#define GOOD_CMD(cmd)					std::string(cmd + ": Good command")