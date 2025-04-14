/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:27:39 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/14 17:22:01 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::pass(int userSocket,const std::string& password)
{
	if(password == _server.getPassword())
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
		return ;
	}
}


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
		_users[userSocket]._buffer.clear();
		return;
	}
	else if (subCommand == "REQ")
	{
		std::cout << GREEN << "REQ" << std::endl << RESET;
		std::getline(iss, params);
		std::string	capResponse = "CAP * ACK :multi-prefix\r\n"; // Andere Response möglich ??? oer msus das
		sendTo(userSocket, capResponse);
		_users[userSocket]._buffer.clear();
		return;
	}
	else if (subCommand == "END")
	{
		std::cout << GREEN << "END" << std::endl << RESET;
		_users[userSocket]._loginProcess = "END";
		return;
	}

	if (command == "PASS")
	{
		std::cout << GREEN << "PASS" << std::endl << RESET;
		if (subCommand[0] == ':')
			subCommand.erase(0, 1);
		pass(userSocket, subCommand);
	}

	if (params.empty() && _users[userSocket]._registered == true)
	{
		_server.welcomeMsg(userSocket);
		_users[userSocket]._loginProcess = "DONE";
	}

	// _users[userSocket]._registered = true;
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
