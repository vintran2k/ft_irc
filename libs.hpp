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
#include <fcntl.h>
#include "colors.hpp"

class Server;
class Client;