/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:39 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/14 13:31:00 by Dscheffn         ###   ########.fr       */
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
		sendTo(userSocket, capResponse);
		return;
	}
	else if (subCommand == "REQ")
	{
		std::cout << GREEN << "REQ" << std::endl << RESET;
		// Nickname, Passwort, USER Name auslesen und einspeichern und zurücksenden
		// gescheit Parsen
		std::getline(iss, params);
		std::string	capResponse = "CAP * ACK :multi-prefix\r\n"; // Andere Response möglich ??? oer msus das
		sendTo(userSocket, capResponse);
		return;
	}
	else if (subCommand == "END")
	{
		std::cout << GREEN << "END" << std::endl << RESET;
		iss >> _users[userSocket]._password >> tmp >> _users[userSocket]._nickname;
		iss >> tmp >> _users[userSocket]._userName >> tmp >> _users[userSocket]._hostName >> _users[userSocket]._realName;
		if (_server.usersExists(_users[userSocket]._nickname))
		{
			sendTo(userSocket, ERR_NICKNAMEINUSE(_users[userSocket]._nickname));
			return ;
		}
		_users[userSocket]._realName.erase(0, 6);
		_users[userSocket]._password.erase(0, 1);
		_users[userSocket]._mask = _users[userSocket]._nickname + "!" + _users[userSocket]._userName + "@" + _users[userSocket]._hostName;
		std::cout << "Nickname: " << _users[userSocket]._nickname << std::endl;
		std::cout << "Password: " << _users[userSocket]._password << std::endl;
		std::cout << "Username: " << _users[userSocket]._userName << std::endl;
		std::cout << "Realname: " << _users[userSocket]._realName << std::endl;
		_users[userSocket]._loginProcess = "END";
		return;
	}


	if (_users[userSocket]._password != _server.getPassword())
	{
		//kick user
	}
	if (_users[userSocket]._loginProcess == "END")
	{
		// if (_users[userSocket]._password == ) // NC fixen!
		// {
		// 	sendTo(userSocket, ERR_NONICKNAMEGIVEN());
		// 	return ;
		// }
		_server.welcomeMsg(userSocket);
		_users[userSocket]._loginProcess = "DONE";
	}

	std::string	token;
	while (iss >> token)
	{
		std::cout << GREEN << "token:" << token << std::endl << RESET;
	}


}

bool	Server::usersExists(std::string nickname)
{
	int i = 0;
	std::map<int, User>::iterator it = _users.begin();
	for (; it != _users.end(); ++it)
	{
		if (it->second._nickname == nickname)
			i++;
		if (i > 1)
		{
			_users.erase(it->second._socket);
			return true;
		}
	}
	return false;
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
