/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:39 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/02 13:07:07 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// CAP is the first command that a client sends to the server after getting connected
void	Commands::cap(int userSocket, const std::string& message)
{
	std::cout << MAGENTA << "CAP" << std::endl << RESET;

	// heck if user doesnt exist

	std::istringstream	iss(message);
	std::string			command, subCommand, params;

	iss >> command >> subCommand >> params; //Read first and second word

	std::cout << RED << "iss:" << iss.str() << std::endl;
	std::cout << RED << "com:" << command << std::endl;
	std::cout << RED << "sub:" << subCommand << std::endl;
	std::cout << RED << "par:" << params << std::endl << RESET;
	if (subCommand == "LS")
	{
		std::cout << GREEN << "LS" << std::endl << RESET;

		std::string	capResponse = "CAP * LS :multi-prefix sasl\r\n";
		_server.sendTo(userSocket, capResponse);
		return;
	}
	else if (subCommand == "REQ")
	{
		std::cout << GREEN << "REQ" << std::endl << RESET;
		std::getline(iss, params);
		std::string	capResponse = "CAP * ACK " + params + "\r\n";
		_server.sendTo(userSocket, capResponse);
		return;
	}
	else if (subCommand == "END")
	{
		std::cout << GREEN << "END" << std::endl << RESET;
		_users[userSocket]._loginProcess = "END";
		return;
	}

	if (_users[userSocket]._loginProcess == "END")
	{
		std::cout << "Hello World" << std::endl;
		_users[userSocket]._loginProcess = "DONE";
	}

	std::string	token;
	while (iss >> token)
	{
		std::cout << GREEN << "token:" << token << std::endl << RESET;
	}

	/////test
	std::string	welcomeMessage = RPL_WELCOME(_users[userSocket]._nickname);
	_server.sendTo(userSocket, welcomeMessage);

	// 002 - Host info
	std::string yourHost = ":irc.server.com 002 " + _users[userSocket]._nickname + " :Your host is irc.server.com  running version 1.0" + CRLF;

	// 003 - Server creation time
	std::string created = ":irc.server.com 003 " + _users[userSocket]._nickname + " :This server was created today" + CRLF;

	// 004 - Server details
	std::string myInfo = ":irc.server.com 004 " + _users[userSocket]._nickname + " irc.server.com 1.0 iov" + CRLF;

	// send(userSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	_server.sendTo(userSocket, yourHost);
	_server.sendTo(userSocket, created);
	_server.sendTo(userSocket, myInfo);
	/////test

}

//         #Test#Message: CAP LS
// PING :127.0.0.1

//         #Test#Command: CAP
// CAP
// User 4 sent: PING Nick4 irc.server.com

//         #Test#Message: PING Nick4 irc.server.com

//         #Test#Command: PING
// PING
// PONGNick4 irc.server.com

// User 4 sent: JOIN #test

//         #Test#Message: JOIN #test

//         #Test#Command: JOIN
// JOIN
// #test
// Nick4!@127.0.0.1 JOIN :#test
// :Nick4!@127.0.0.1 JOIN #test
