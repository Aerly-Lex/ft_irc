/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:26:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/18 15:06:56 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::nick(int userSocket, const std::string& newNickname)
{

	// check if nickname is valid/empty
	if (newNickname.empty())
	{
		std::cout << RED << "nonicknamegiven error" << std::endl << RESET;
		// send ERR_NOnicknameGIVEN
		return;
	}

	// check if nickname(user) exists
	if (_users.find(userSocket) == _users.end())
	{
		std::cout << RED << "notregistered error" << std::endl << RESET;
		// send ERR_NOTREGISTERED
		return;
	}

	// check if nickname is already in use
	// for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	// {
	// 	if (it->second._nickname == nickname)
	// 	{
			// std::cout << RED << "nicknameinuse error" << std::endl << RESET;
	// 		// send ERR_nicknameINUSE
	// 		return;
	// 	}
	// }
	for (const auto&  pair : _users)
	{
		if (pair.second._nickname == newNickname)
		{
			std::cout << RED << "nicknameinuse error" << std::endl << RESET;
			// send ERR_nicknameINUSE
			return;
		}
	}
	std::string oldNickname = _users[userSocket]._nickname;
	_users[userSocket]._nickname = newNickname;

	// send NICK changes message to client
	std::cout << "User " << userSocket << " set nickname to: " << newNickname << std::endl;
	std::cout << MAGENTA << "SENDING TO CLIENT: " << RPL_NICKCHANGE(oldNickname, newNickname) << std::endl << RESET;
	send(userSocket, RPL_NICKCHANGE(oldNickname, newNickname).c_str(), RPL_NICKCHANGE(oldNickname, newNickname).size(), 0);

	// prob broadcast to all channels
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->first != userSocket)
		{
			std::cout << MAGENTA << "SENDING TO USERS: " << RPL_NICKCHANGE(oldNickname, newNickname) << std::endl << RESET;
			send(it->first, RPL_NICKCHANGE(oldNickname, newNickname).c_str(), RPL_NICKCHANGE(oldNickname, newNickname).size(), 0);
		}
	}
}