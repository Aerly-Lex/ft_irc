/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:26:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/18 13:08:26 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::nick(int userSocket, const std::string&nickName)
{

	// check if nickname is valid/empty
	if (nickName.empty())
	{
		// send ERR_NONICKNAMEGIVEN
		return;
	}

	// check if nickname(user) exists
	if (_users.find(userSocket) == _users.end())
	{
		// send ERR_NOTREGISTERED
		return;
	}

	// check if nickname is already in use
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second._nickName == nickName)
		{
			// send ERR_NICKNAMEINUSE
			return;
		}
	}
	std::string oldNick = _users[userSocket]._nickName;
	_users[userSocket]._nickName = nickName;

	// send NICK changes message to client
	std::cout << "User " << userSocket << " set nickname to: " << nickName << std::endl;
	send(userSocket, RPL_NICKCHANGE(_users[userSocket]._nickName, nickName).c_str(), RPL_NICKCHANGE(_users[userSocket]._nickName, nickName).size(), 0);

	// prob broadcast to all channels
}