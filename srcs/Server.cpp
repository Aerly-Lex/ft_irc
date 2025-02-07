/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:32 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/07 10:33:46 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

//////////////////////////////////////////
//				extern definitions		//
//////////////////////////////////////////

bool	Server::Signal = false;


//////////////////////////////////////////
//				Constructor				//
//////////////////////////////////////////

Server::Server(int port, std::string password) : _port(port), _password(password)
{

}

Server::~Server()
{
	close(_socket);
	// close all clients/channels?
}

Server::Server(Server const & src)
{
	*this = src;
}

Server& Server::operator=(Server const & src)
{
	return *this;
}

//////////////////////////////////////////
//				getter					//
//////////////////////////////////////////

int			Server::getSocket() const
{
	return (_socket);
}

int			Server::getPort() const
{
	return (_port);
}

std::string	Server::getPassword() const
{
	return (_password);
}

//////////////////////////////////////////
//				class methods			//
//////////////////////////////////////////

void	Server::signalHandler(int signum)
{
	// (void)signum; // Supress unused variable warning
	Server::Signal = true; // Set the signal flag to true
	std::cout << RED << "Interrupt signal (" << signum << ") received. Exiting..." << RESET << std::endl;
}


void		Server::initServer()
{
	// implement signals for the server
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);

	// Create a socket
	// (protocol family)PF_INET/(address family)AF_INET: IPv4 | SOCK_STREAM: TCP - Two-way communication | 0: chooses protocol automatically (based on type)
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Socket creation failed");

	// Bind the socket to an address
	sockaddr_in	serverAddress;
	serverAddress.sin_family = AF_INET; // AF_INET: IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY: any address for binding
	serverAddress.sin_port = htons(_port); // htons: host to network short converts values between host and network (network-format)

//	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("Failed to bind the socket to the address");


//	int listen(int sockfd, int backlog);
//	backlog: The maximum length queue of pending connections for sockfd may grow.
//	SOMAXCONN: maximum value for the backlog argument
//	The listen() call applies only to sockets of type SOCK_STREAM.
	if (listen(_socket, SOMAXCONN) == -1)
		throw std::runtime_error("Failed to listen for incoming connections/socket");

	std::cout << CYAN << "Server is listening on port " << _port << RESET << std::endl;
}

void		Server::run()
{
	pollfd fds[1];
	FD_ZERO(&fds[0]);
	FD_SET(_socket, &fds[0]);
	while (Server::Signal == false)
	{
		int ret = poll(fds.data(), fds.size(), -1);
	}

}

//      int poll(struct pollfd fds[], nfds_t nfds, int timeout);
// DESCRIPTION
//      poll() examines a set of file descriptors to see if some of them are ready for I/O or if certain events have occurred on them.  The fds argument is a pointer to an
//      array of pollfd structures, as defined in <poll.h> (shown below).  The nfds argument specifies the size of the fds array.

//      struct pollfd {
//          int    fd;       /* file descriptor */
//          short  events;   /* events to look for */
//          short  revents;  /* events returned */
//      };

//      The fields of struct pollfd are as follows:

//      fd             File descriptor to poll.

//      events         Events to poll for.  (See below.)

//      revents        Events which may occur or have occurred.  (See below.)

//      If timeout is greater than zero, it specifies a maximum interval (in milliseconds) to wait for any file descriptor to become ready.  If timeout is zero, then poll()
//      will return without blocking. If the value of timeout is -1, the poll blocks indefinitely.

// RETURN VALUES
//      poll() returns the number of descriptors that are ready for I/O, or -1 if an error occurred.  If the time limit expires, poll() returns 0.  If poll() returns with an
//      error, including one due to an interrupted call, the fds array will be unmodified and the global variable errno will be set to indicate the error.