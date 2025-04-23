/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:39 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 14:18:41 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::pass(int userSocket,const std::string& password)
{
	if (password == _server.getPassword())
	{
		std::cout << MAGENTA << "PASS: " << password << std::endl << RESET;
		_users[userSocket]._password = password;
		_users[userSocket]._loginProcess = "END";
		_users[userSocket]._registered = true;
	}
	else
	{
		std::cout << RED << "Wrong Password" << std::endl << RESET;
		sendTo(userSocket, ERR_PASSWDMISMATCH(_users[userSocket]._nickname));
		shutdown(userSocket, SHUT_RDWR);
		close(userSocket);
		_users.erase(userSocket);
	}
}

// CAP is the first command that a client sends to the server after getting connected
void	Commands::cap(int userSocket, const std::string& message)
{
	std::istringstream	iss(message);
	std::string			command, subCommand, params;

	iss >> command >> subCommand >> params;

	if (subCommand == "LS")
	{
		std::string	capResponse = "CAP * LS :multi-prefix sasl\r\n";
		sendTo(userSocket, capResponse);
		_users[userSocket]._buffer.clear();
		return;
	}
	else if (subCommand == "REQ")
	{
		std::getline(iss, params);
		std::string	capResponse = "CAP * ACK :multi-prefix\r\n";
		sendTo(userSocket, capResponse);
		_users[userSocket]._buffer.clear();
		return;
	}
	else if (subCommand == "END")
	{
		_users[userSocket]._loginProcess = "END";
		return;
	}

	if (command == "PASS" && _users[userSocket]._loginProcess == "END")
	{
		if (subCommand[0] == ':')
			subCommand.erase(0, 1);
		pass(userSocket, subCommand);
	}
	else if( _users[userSocket]._loginProcess == "END")
	{
		std::cout << YELLOW << "User not registered yet" << std::endl << RESET;
		return sendTo(userSocket, "Please enter Password!\r\n");
	}

	if (params.empty() && _users[userSocket]._registered == true)
	{
		if (!_users[userSocket]._buffer.empty() && params == "NICK")
		{
			iss >> _users[userSocket]._nickname;
		}
		_server.welcomeMsg(userSocket);
		_users[userSocket]._loginProcess = "DONE";
	}
}
