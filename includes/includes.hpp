#pragma once

// C++ includes

#include <sstream>
#include <iostream>
#include <sys/types.h>		// socket, bind, connect, listen, accept, lseek, fstat
#include <sys/socket.h>		// socket, bind, connect, listen, accept, setsockopt, getsockname
#include <netinet/in.h>		// htons, htonl, ntohs, ntohl, sockaddr_in
#include <arpa/inet.h>		// inet_addr, inet_ntoa
#include <netdb.h>			// getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo
#include <unistd.h>			// close, lseek
#include <string.h>			// memset, memcpy
#include <fcntl.h>			// fcntl
#include <sys/stat.h>		// fstat
#include <signal.h>			// signal, sigaction
#include <poll.h>			// poll
#include <map>				// std::map
#include <vector>			// std::vector
#include <set>
#include <chrono>			// save creation time

// #include <fd_set.h>			// FD_ZERO, FD_SET

// Custom includes

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "replies.hpp"

// ANSI Colors

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// Messages defines

#define invalidArguments	"Error: Wrong number of arguments" << std::endl << "Usage: ./webserv [port] [password]"
#define invalidPort			"Error: Port number must be between 1024 and 49151" << std::endl
