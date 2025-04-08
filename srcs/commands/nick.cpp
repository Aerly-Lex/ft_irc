/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:26:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/08 16:55:39 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::nick(int userSocket, const std::string& newNickname)
{
	// check if nickname is valid/empty
	if (newNickname.empty())
		return sendTo(userSocket, ERR_NONICKNAMEGIVEN());

	// check if nickname(user) exists
	if (_users.find(userSocket) == _users.end())
		return sendTo(userSocket, ERR_NOTREGISTERED());

	// check if nickname is already in use
	for (const auto& [sock, user] : _users)
	{
		if (user._nickname == newNickname)
			return sendTo(userSocket, ERR_NICKNAMEINUSE(newNickname));
	}
	std::string oldNickname = _users[userSocket]._nickname;
	_users[userSocket]._nickname = newNickname;

	// send NICK changes message to client
	std::cout << "User " << userSocket << " set nickname to: " << newNickname << std::endl;
	std::cout << MAGENTA << "SENDING TO CLIENT: " << RPL_NICKCHANGE(oldNickname, newNickname) << std::endl << RESET;
	sendTo(userSocket, RPL_NICKCHANGE(oldNickname, newNickname));

	// prob broadcast to all channels
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->first != userSocket)
		{
			std::cout << MAGENTA << "SENDING TO USERS: " << RPL_NICKCHANGE(oldNickname, newNickname) << std::endl << RESET;
			sendTo(it->first, RPL_NICKCHANGE(oldNickname, newNickname));
		}
	}
}
