/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:32 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:45 by stopp            ###   ########.fr       */
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

Server::Server(int port, std::string password) : _port(port), _password(password), _commands(*this), _creationTime(std::chrono::system_clock::now())
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

std::string		Server::getCreationTime() const
{
	std::time_t now = std::chrono::system_clock::to_time_t(_creationTime);
	std::string timeStr = std::ctime(&now);
	timeStr.pop_back();
	return timeStr;
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

void	Server::welcomeMsg(int userSocket)
{
	sendTo(userSocket, RPL_WELCOME(_users[userSocket]._nickname));
	sendTo(userSocket, RPL_YOURHOST(_users[userSocket]._nickname));
	sendTo(userSocket, RPL_CREATED(_users[userSocket]._nickname, getCreationTime()));
}

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

	//this makes sure we can reuse the adress/port after it closes instead of waiting for a timeout.
	int re = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re)) == -1)
		throw(std::runtime_error("ERROR: setsockopt failed"));

//	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if (bind(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("Failed to bind the socket to the address");

	//making it nonblocking on MacOS:
	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("ERROR: fcntl failed"));

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
	newUser._nickname = "Nick" + std::to_string(userSocket);
	newUser._userName = "";
	newUser._realName = "";
	newUser._password = "";
	newUser._hostName = inet_ntoa(userAddr.sin_addr); // saving IP-Adress as hostname
	// newUser._hostName = "";

	// newUser._ipAddress = inet_ntoa(userAddr.sin_addr); // saving IP-Adress
	newUser._registered = false;
	_users[userSocket] = newUser;

	// adding the new user socket to poll-fd list
	pollfd	user_fd;
	user_fd.fd = userSocket;
	user_fd.events = POLLIN;
	fds.push_back(user_fd);


	// /////test
	// std::string	welcomeMessage = RPL_WELCOME(_users[userSocket]._nickname);
	// send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	// // 002 - Host info
	// std::string yourHost = ":irc.server.com 002 " + newUser._nickname + " :Your host is irc.server.com  running version 1.0" + CRLF;

	// // 003 - Server creation time
	// std::string created = ":irc.server.com 003 " + newUser._nickname + " :This server was created today" + CRLF;

	// // 004 - Server details
	// std::string myInfo = ":irc.server.com 004 " + newUser._nickname + " irc.server.com 1.0 iov" + CRLF;

	// // send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	// send(userSocket, yourHost.c_str(), yourHost.size(), 0);
	// send(userSocket, created.c_str(), created.size(), 0);
	// send(userSocket, myInfo.c_str(), myInfo.size(), 0);
	// /////test

	std::cout << "New User connected: " << userSocket << std::endl;
	std::cout << "Total Users: " << fds.size() - 1 << std::endl;
}

void	Server::handleUserMessage(std::vector<pollfd>& fds, int i)
{
	char	buffer[1024] = {0};
	int		bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);

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
	std::cout << "User " << _users[fds[i].fd]._nickname << " sent: " << message << std::endl;

	handleUserCommand(fds[i].fd, message);
}
// finds the target for the message and returns either the userSocket, -1 if its an existing channel or 0 if no target is found
int		Server::findTarget(const std::string &target)
{
	for (auto it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second._nickname == target)
			return (it->first);
	}
	if (_channels.find(target) != _channels.end())
		return (-1);
	else
		return (0);
}

void	Server::handleUserCommand(int userSocket, const std::string& message)
{
	std::istringstream	iss(message);
	std::string			command;
	iss >> command;

	std::cout << MAGENTA <<  "\t#Test#Message: " << message << std::endl;
	std::cout << "\t#Test#Command: " << command << std::endl << RESET;

	if (command == "CAP" || _users[userSocket]._loginProcess == "END")
		_commands.cap(userSocket, message);
	else if (command == "JOIN")
	{
		std::cout << MAGENTA << "JOIN" << std::endl << RESET;
		std::string	channelName;
		iss >> channelName;
		std::cout << channelName << std::endl;
		_commands.join(userSocket, channelName);
	}
	else if (command == "NICK")
	{
		std::string	nickName;
		iss >> nickName;
		std::cout << MAGENTA << "NICK: " << nickName << std::endl << RESET;
		_commands.nick(userSocket, nickName);
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
	else if (command == "PRIVMSG")
	{
		std::string target, message;
		iss >> target;
		message = iss.rdbuf()->str();
		_commands.privmsg(userSocket, _users[userSocket]._mask, target, message);
	}
	else
		(void)command; //unkown command RPL 421
}

void sendTo(int fd, const std::string &message)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Failed to send to fd: " << fd << std::endl;
	}
}
