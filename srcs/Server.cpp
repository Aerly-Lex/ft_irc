/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:32 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 16:40:09 by Dscheffn         ###   ########.fr       */
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
: _port(port), _password(password), _commands(*this), _creationTime(std::chrono::system_clock::now())
{

}

Server::~Server()
{
	for (size_t i = 0; i < _fds.size(); ++i)
		close(_fds[i].fd);
	std::cout << GREEN << "Server shut down cleanly." << RESET << std::endl;
	close(_socket);
}

//////////////////////////////////////////
//				getter					//
//////////////////////////////////////////

int								Server::getSocket() const { return (_socket); }
int								Server::getPort() const { return (_port); }
std::string						Server::getPassword() const { return (_password); }
std::map<int, User>&			Server::getUsers() { return (_users); }
std::map<std::string, Channel>&	Server::getChannels() { return (_channels); }

std::string		Server::getCreationTime() const
{
	std::time_t now = std::chrono::system_clock::to_time_t(_creationTime);
	std::string timeStr = std::ctime(&now);
	timeStr.pop_back();
	return timeStr;
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
	const char	*msg = "\nInterrupt signal received. Exiting...\n";
	write(STDOUT_FILENO, msg, strlen(msg)); // because std::cout is buffered and would not flush
	Server::Signal = true;
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
	int	re = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(re)) == -1)
		throw(std::runtime_error("ERROR: setsockopt failed"));

	if (bind(_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("Failed to bind the socket to the address");

	// making it nonblocking on MacOS:
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
		{
			if (errno == EINTR) // Signal interrups poll
				continue;		// Keep going until the signal is handled
			throw std::runtime_error("Poll failed");
		}

		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == _socket)
					acceptNewUsers(fds);
				else
					handleUserMessage(fds, i);
			}
			else if (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				int deadSocket = fds[i].fd;
				std::cout << RED << "Socket " << deadSocket << " said goodbye." << RESET << std::endl; // can be removed later
				removeUserFromAllChannels(deadSocket);
				close(deadSocket);
				fds.erase(fds.begin() + i);
				i--;
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

	newUser._registered = false;
	_users[userSocket] = newUser;

	// adding the new user socket to poll-fd list
	pollfd	user_fd;
	user_fd.fd = userSocket;
	user_fd.events = POLLIN;
	fds.push_back(user_fd);

	std::cout << "New User connected: " << userSocket << std::endl;
	std::cout << "Total Users: " << fds.size() - 1 << std::endl;
}

void Server::removeUserFromAllChannels(int socket)
{
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		it->second.removeMember(socket);
	_users.erase(socket);
}

void Server::handleUserMessage(std::vector<pollfd>& fds, int i)
{
	char buffer[1024];
	int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);

	if (bytesRead <= 0)
	{
		std::cout << "User " << fds[i].fd << " disconnected" << std::endl;
		close(fds[i].fd);
		_users.erase(fds[i].fd);
		fds.erase(fds.begin() + i);
		return;
	}

	if (buffer[bytesRead - 1] != '\n')
	{
		std::cerr << RED << "Message not null-terminated" << RESET << std::endl; // kann entfernt werden - tester mess
		_users[fds[i].fd]._buffer += std::string(buffer, bytesRead);
		std::cerr << RED << "BUFFER: " << _users[fds[i].fd]._buffer << RESET << std::endl; // kann entfernt werden - tester mess
		return;
	}

	_users[fds[i].fd]._buffer += std::string(buffer, bytesRead);

	// Process all complete commands (ending with \r\n)
	size_t pos;
	while ((pos = _users[fds[i].fd]._buffer.find("\r\n")) != std::string::npos)
	{
		std::string line = _users[fds[i].fd]._buffer.substr(0, pos);
		_users[fds[i].fd]._buffer.erase(0, pos + 2);

		std::cout << MAGENTA << "COMMAND: " << line << RESET << std::endl;
		handleUserCommand(fds[i].fd, line);
	}
}


// finds the target for the message and returns either the userSocket
// -1 if its an existing channel | 0 if no target is found
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

void Server::handleUserCommand(int userSocket, const std::string& message)
{
	std::istringstream iss(message);
	std::string command;
	iss >> command;

	std::cout << MAGENTA << "\t#Test#Message: " << message << std::endl;
	std::cout << "\t#Test#Command: " << command << std::endl << RESET;

	if (_users[userSocket]._registered == false)
	{
		std::cout << YELLOW << "User not registered yet" << std::endl << RESET;
		_commands.cap(userSocket, message);
	}
	else if (command == "JOIN")
	{
		std::string channelName, password;
		iss >> channelName >> password;
		_commands.join(userSocket, channelName, password);
	}
	else if (command == "NICK")
	{
		std::string nickName;
		iss >> nickName;
		_commands.nick(userSocket, nickName);
	}
	else if (command == "PING")
	{
		std::string token;
		iss >> token;
		_commands.ping(userSocket);
	}
	else if (command == "KICK")
	{
		std::string channelName, nickName, reason;
		iss >> channelName >> nickName >> reason;
		_commands.kick(userSocket, channelName, nickName, reason);
	}
	else if (command == "INVITE")
	{
		std::string target, channel;
		iss >> target >> channel;
		_commands.invite(userSocket, target, channel);
	}
	else if (command == "TOPIC")
	{
		std::string channel, topic;
		iss >> channel;
		std::getline(iss, topic);
		if (!topic.empty())
			topic.erase(0, 2);
		_commands.topic(userSocket, channel, topic);
	}
	else if (command == "USER")
	{
		std::string userName, tmp, realName;
		iss >> _users[userSocket]._nickname >> tmp >> tmp >> _users[userSocket]._realName >> realName;
		_users[userSocket]._realName.erase(0, 6);
		_users[userSocket]._realName += " " + realName;
		welcomeMsg(userSocket);
	}
	else if (command == "MODE")
	{
		std::string channel, flag, param;
		std::vector<std::string> params;
		iss >> channel >> flag;
		while (iss >> param)
			params.push_back(param);
		_commands.mode(userSocket, channel, flag, params);
	}
	else if (command == "QUIT")
	{
		_commands.quit(userSocket);
	}
	else if (command == "PART")
	{
		std::string channel;
		iss >> channel;
		_commands.part(userSocket, channel);
	}
	else if (command == "PRIVMSG")
	{
		std::string target, messageContent;
		iss >> target;
		std::getline(iss, messageContent);
		_commands.privmsg(userSocket, _users[userSocket]._mask, target, messageContent);
	}
	else if (!command.empty())
	{
		std::cout << MAGENTA << "Unknown command received: [" << command << "]" << RESET << std::endl;
		sendTo(userSocket, ERR_UNKNOWNCOMMAND(_users[userSocket]._nickname, command));
	}
	if (!_users[userSocket]._buffer.empty() && !isOnlyWhitespace(_users[userSocket]._buffer))
	{
		size_t pos = _users[userSocket]._buffer.find('\n');
		if (pos != std::string::npos)
			_users[userSocket]._buffer.erase(0, pos + 1);
		std::cout << MAGENTA << "BUFFER: " << _users[userSocket]._buffer << "ENDOFBUFFER" << RESET << std::endl;
		handleUserCommand(userSocket, _users[userSocket]._buffer);
	}
	_users[userSocket]._buffer.clear();
}

// Alte Funktion - falls die neue wie mit KVirc nicht funktioniert
/*
void	Server::handleUserCommand(int userSocket, const std::string& message)
{
	std::istringstream	iss(message);
	std::string			command;
	iss >> command;
	if (command.empty())
		return; // empty command, do nothing
	std::cout << MAGENTA <<  "\t#Test#Message: " << message << std::endl;	//tester kann entferntw werden
	std::cout << "\t#Test#Command: " << command << std::endl << RESET;	//tester kann entferntw werden

	if (_users[userSocket]._registered == false)
	{
		std::cout << YELLOW << "User not registered yet" << std::endl << RESET; // tester kann entfernt werden
		_commands.cap(userSocket, message);
	}
	else if (command == "JOIN")
	{
		std::cout << MAGENTA << "JOIN" << std::endl << RESET;
		std::string	channelName, password;
		iss >> channelName >> password;
		_commands.join(userSocket, channelName, password);
	}
	else if (command == "NICK") // changes or sets the nickname of the user
	{
		std::string	nickName;
		iss >> nickName;
		std::cout << MAGENTA << "NICK: " << nickName << std::endl << RESET;
		_commands.nick(userSocket, nickName);
	}
	else if (command == "PING") // ping the server to check if its alive
	{
		std::cout << MAGENTA << "PING" << std::endl << RESET;
		std::string	token;
		iss >> token;
		_commands.ping(userSocket);
	}
	else if (command == "KICK")
	{
		std::string	chnlName, nickName, reason;
		iss >> chnlName >> nickName >> reason;
		_commands.kick(userSocket, chnlName, nickName, reason);
	}
	else if (command == "INVITE") 	// invits a user to a channel
	{
		std::string	target, channel;
		iss >> target >> channel;
		_commands.invite(userSocket, target, channel);
		std::cout << "INVITE" << std::endl;
	}
	else if (command == "TOPIC") 	// changes or gets the topic of a channel
	{
		std::cout << "TOPIC" << std::endl;
		std::string	channel, topic;
		iss >> channel;
		std::getline(iss, topic);
		if (!topic.empty())
			topic.erase(0, 2);
		_commands.topic(userSocket, channel, topic);
	}
	else if (command == "USER") 	// reads the input from the user
	{
		std::string	userName, tmp, realName;
		iss >> _users[userSocket]._nickname >> tmp >> tmp >> _users[userSocket]._realName >> realName ;

		// size_t pos = _users[userSocket]._buffer.find('\n');
		// _users[userSocket]._buffer.erase(0, pos + 1);

		_users[userSocket]._realName.erase(0, 6); // nc bug! doppelpunkt
		_users[userSocket]._realName += " " + realName;
		welcomeMsg(userSocket);
	}
	else if (command == "MODE") 	// sets the mode of a channel and based on the mode sets the usermode
	{
		// channel, flag, and the param == username if needed
		std::string					channel, flag, param;
		std::vector<std::string>	params;
		iss >> channel >> flag;
		while (iss >> param)
			params.push_back(param);
		_commands.mode(userSocket, channel, flag, params);
	}
	else if (command == "QUIT") 	// disconnects the user from the server
	{
		_commands.quit(userSocket);
	}
	else if (command == "PART") 	// leaves a channel
	{
		std::string	channel;
		iss >> channel;
		_commands.part(userSocket, channel);
	}
	else if (command == "PRIVMSG")
	{
		std::string	target, message;
		iss >> target;
		std::getline(iss, message);

		// Remove leading spaces and colons from the message (important for nc console)
		if (!message.empty() && message[0] == ' ')
			message.erase(0, 1);
		if (!message.empty() && message[0] == ':')
			message.erase(0, 1);

		std::cout << RED << "Message: " << message << RESET << std::endl;
		_commands.privmsg(userSocket, _users[userSocket]._mask, target, message);
	}
	// Macht fehler aktuell
	// else // handles unknown commands or commands that are not mandatory
	// {
	// 	std::cout << MAGENTA << "Unknown command: " << command << "$" << RESET << std::endl;
	// 	sendTo(userSocket, ERR_UNKNOWNCOMMAND(_users[userSocket]._nickname, command));
	// }

	if (!_users[userSocket]._buffer.empty()) // !! Whitespaces skip/check to avoid errors from else unkown command
	{
		size_t pos = _users[userSocket]._buffer.find('\n');
		_users[userSocket]._buffer.erase(0, pos + 1);
		std::cout << MAGENTA << "BUFFER: " << _users[userSocket]._buffer << "ENDOFBUFFER" << RESET << std::endl;
		handleUserCommand(userSocket, _users[userSocket]._buffer);
	}
	_users[userSocket]._buffer.clear(); // clear the buffer after processing the message
}
*/
