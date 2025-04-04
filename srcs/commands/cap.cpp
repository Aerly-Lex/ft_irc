/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:39 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/03 18:11:30 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// CAP is the first command that a client sends to the server after getting connected
void	Commands::cap(int userSocket, const std::string& message)
{
	std::cout << MAGENTA << "CAP" << std::endl << RESET;

	// heck if user doesnt exist

	std::istringstream	iss(message);
	std::string			command, subCommand, params, tmp;

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
		// Nickname, Passwort, USER Name auslesen und einspeichern und zurücksenden
		// gescheit Parsen
		std::getline(iss, params);
		std::string	capResponse = "CAP * ACK :multi-prefix\r\n"; // Andere Response möglich ??? oer msus das
		_server.sendTo(userSocket, capResponse);
		return;
	}
	else if (subCommand == "END")
	{
		std::cout << GREEN << "END" << std::endl << RESET;
		iss >> _users[userSocket]._password >> tmp >> _users[userSocket]._nickname;
		iss >> tmp >> _users[userSocket]._userName >> tmp >> tmp >> _users[userSocket]._realName;
		_users[userSocket]._realName.erase(0, 6);
		_users[userSocket]._password.erase(0, 1);
		std::cout << "Nickname: " << _users[userSocket]._nickname << std::endl;
		std::cout << "Password: " << _users[userSocket]._password << std::endl;
		std::cout << "Username: " << _users[userSocket]._userName << std::endl;
		std::cout << "Realname: " << _users[userSocket]._realName << std::endl;
		_users[userSocket]._loginProcess = "END";
		_server.welcomeMsg(userSocket);
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
