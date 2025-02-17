/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:32 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 12:50:55 by Dscheffn         ###   ########.fr       */
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

Server::Server(int port, std::string password)
: _port(port), _password(password), _commands(*this)
{

}

Server::~Server()
{
	close(_socket);
	// close all users/channels?
}

//////////////////////////////////////////
//				getter					//
//////////////////////////////////////////

int				Server::getSocket() const
{
	return (_socket);
}

int				Server::getPort() const
{
	return (_port);
}

std::string		Server::getPassword() const
{
	return (_password);
}

std::map<int, User>&			Server::getUsers()
{
	return (_users);
}

std::map<std::string, Channel>&	Server::getChannels()
{
	return (_channels);
}


//////////////////////////////////////////
//				class methods			//
//////////////////////////////////////////

void	Server::signalHandler(int signum)
{
	// (void)signum; // Supress unused variable warning from www
	Server::Signal	= true; // Set the signal flag to true
	std::cout << RED << "Interrupt signal (" << signum << ") received. Exiting..." << RESET << std::endl;
}

// use AF_INET in your struct sockaddr_in and PF_INET in your call to socket()
void		Server::initServer()
{
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);

	// Create a socket
	// (protocol family)PF_INET/(address family)AF_INET: IPv4 | SOCK_STREAM: TCP - Two-way communication | 0: chooses protocol automatically (based on type)
	// _socket = socket(AF_INET, SOCK_STREAM, 0); // man connect
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Socket creation failed");

	// Bind the socket to an address
	// This structure makes it easy to reference elements of the socket address.
	// Note that sin_zero (which is included to pad the structure to the length of a struct sockaddr) should be set to all zeros with the function memset().
	// Also, notice that sin_family corresponds to sa_family in a struct sockaddr and should be set to “AF_INET”.
	// Finally, the sin_port must be in Network Byte Order (by using htons()!)
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
	std::vector<pollfd>	fds;

	pollfd	serverPoll;
	serverPoll.fd = _socket;	// server socket fd
	serverPoll.events = POLLIN;	// watches for incoming connections
	fds.push_back(serverPoll);	// add server socket to the pollfd vector

	while (Server::Signal == false)
	{
		// poll() blocks till a event occurs
		int ret = poll(fds.data(), fds.size(), -1);
		if (ret == -1)
			throw std::runtime_error("Poll failed");

		for (size_t i = 0; i < fds.size(); i++) // to slim the function
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == _socket)
					acceptNewUsers(fds);
				else
					handleUserMessage(fds, i);
			}
		}
	}
}

void	Server::acceptNewUsers(std::vector<pollfd>& fds)
{
	sockaddr_in	userAddr;
	socklen_t	userAddrLen = sizeof(userAddr);
	int userSocket = accept(_socket, (sockaddr*)&userAddr, &userAddrLen);
	if (userSocket == -1)
		throw std::runtime_error("Failed to accept incoming connection");

	// create a new User object and add to map
	User	newUser(userSocket);
	newUser._socket = userSocket;
	newUser._nickName = "Nick" + std::to_string(userSocket);
	newUser._userName = "";
	newUser._realName = "";
	newUser._password = "";
	// newUser._hostName = inet_ntoa(clientAddr.sin_addr); // saving IP-Adress as hostname
	newUser._hostName = "";
	// newUser._ipAddress = inet_ntoa(clientAddr.sin_addr); // saving IP-Adress
	newUser._registered = false;
	_users[userSocket] = newUser;

	// adding the new user socket to poll-fd list
	pollfd	user_fd;
	user_fd.fd = userSocket;
	user_fd.events = POLLIN;
	fds.push_back(user_fd);


	// create a general channel if it doesn't exist
	// if (_channels.empty())
	// {
	// 	Channel	defaultChannel("general");
	// 	_channels["general"] = defaultChannel;

	// 	_channels["general"].members.push_back(userSocket);

	// 	std::cout << GREEN << "New user connected: " << userSocket << std::endl << RESET;
	// 	std::cout << "Total user: " << fds.size() - 1 << std::endl;

	// 	std::string welcomeMessage = RPL_WELCOME(_users[userSocket]._nickName);
	// 	send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);

	// 	std::string	memberList = "Members in #general: ";
	// 	for (int member : _channels["general"].members)
	// 		memberList += std::to_string(member) + " ";
	// 	memberList += "\r\n";
	// 	send(userSocket, memberList.c_str(), memberList.size(), 0);

	// 	// _channels["general"] = Channel("general");
	// 	// std::cout << GREEN << "Created channel: general" << std::endl << RESET;
	// 	// std::string joinMessage = ":127.0.0.1 MODE #test +o Nickname2";
	// 	// send(userSocket, joinMessage.c_str(), joinMessage.size(), 0);
	// // :127.0.0.1 MODE #test +o Nickname2
	// 	// send(userSocket, joinMessage.c_str(), joinMessage.size(), 0);
	// }
	// std::string welcomeMessage = ":irc.server.com 001 " + newUser._nickName + " :Welcome to the 42 IRC server!\r\n";

	/////test
	std::string	welcomeMessage = RPL_WELCOME(_users[userSocket]._nickName);
	send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	// 002 - Host info
	std::string yourHost = ":irc.server.com 002 " + newUser._nickName + " :Your host is irc.server.com  running version 1.0" + CRLF;

	// 003 - Server creation time
	std::string created = ":irc.server.com 003 " + newUser._nickName + " :This server was created today" + CRLF;

	// 004 - Server details
	std::string myInfo = ":irc.server.com 004 " + newUser._nickName + " irc.server.com 1.0 iov" + CRLF;

	// send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	send(userSocket, yourHost.c_str(), yourHost.size(), 0);
	send(userSocket, created.c_str(), created.size(), 0);
	send(userSocket, myInfo.c_str(), myInfo.size(), 0);
	/////test

	std::cout << "New User connected: " << userSocket << std::endl;
	std::cout << "Total Users: " << fds.size() - 1 << std::endl;
}

void	Server::handleUserMessage(std::vector<pollfd>& fds, int i)
{
	char	buffer[1024] = {0};
	int		bytesRead = recv(fds[i].fd, buffer, 1024, 0);

	if (bytesRead <= 0) // 0 for disconnection, -1 for error
	{
		std::cout << "User " << fds[i].fd << " disconnected" << std::endl;
		close(fds[i].fd);
		_users.erase(fds[i].fd); // remove the user from the map
		fds.erase(fds.begin() + i); // remove the user from the pollfd list
		// i--;
		return;
	}

	buffer[bytesRead] = '\0'; // null-terminate the buffer
	std::string	message(buffer);
	std::cout << "User " << fds[i].fd << " sent: " << buffer << std::endl;

	handleUserCommand(fds[i].fd, message);
}

void	Server::handleUserCommand(int userSocket, const std::string& message)
{
	std::istringstream	iss(message);
	std::string			command;
	iss >> command;

	std::cout << MAGENTA <<  "\t#Test#Message: " << message << std::endl;
	std::cout << "\t#Test#Command: " << command << std::endl << RESET;
	if (command == "JOIN")
	{
		std::cout << MAGENTA << "JOIN" << std::endl << RESET;
		std::string	channelName;
		iss >> channelName;
		std::cout << channelName << std::endl;
		_commands.join(userSocket, channelName);
	}
	else if (command == "NICK")
	{
		std::cout << MAGENTA << "NICK" << std::endl << RESET;
		std::string	nickName;
		iss >> nickName;
		_commands.nick(userSocket, message);
	}
	else if (command == "PING")
	{
		std::cout << MAGENTA << "PING" << std::endl << RESET;
		std::string	token;
		iss >> token;
		_commands.ping(userSocket);
	}
	else if (command == "KICK")
		(void)command;
	else if (command == "INVITE")
		std::cout << "INVITE" << std::endl;
	else if (command == "TOPIC")
		std::cout << "TOPIC" << std::endl;
	else if (command == "MODE")
		(void)command;
	else if (command == "QUIT")
		_commands.quit(userSocket);
	else if (command == "PART") // part == leave channel
		_commands.part(userSocket, message);
	else
		(void)command; //unkown command

	// else if (command == "USER") {
	// 	std::string userName, realName;
	// 	iss >> userName;
	// 	iss.ignore(256, ' '); // Ignoriere den Hostnamen
	// 	iss.ignore(256, ' '); // Ignoriere den Servernamen
	// 	std::getline(iss, realName, ':'); // Realname beginnt nach dem Doppelpunkt
	// 	// _commands.user(fds[i].fd, userName, realName);
	// }


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