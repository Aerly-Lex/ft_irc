/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:20 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/02 13:07:34 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::join(int userSocket, const std::string& channelName)
{
	// Check if channel exists, if not create it
	if (_server.getChannels().find(channelName) == _server.getChannels().end())
	{
		Channel	newChannel(channelName);
		newChannel._name = channelName;
		_server.getChannels()[channelName] = newChannel;
	}
	// Add client to channel
	_server.getChannels()[channelName].members.push_back(userSocket);

	// Send JOIN message to client
	// std::string	joinMsg = ":" + _clients[clientSocket]._nickName + " JOIN :" + channelName + CRLF;
	std::cout << _users[userSocket]._nickname + "!" + _users[userSocket]._userName + "@" + _users[userSocket]._hostName + " JOIN :" + channelName << std::endl;
	std::string joinMsg = RPL_JOINMSG(_users[userSocket]._nickname, _users[userSocket]._userName, _users[userSocket]._hostName, channelName);
	std::cout << RED << joinMsg << std::endl << RESET;
	_server.sendTo(userSocket, joinMsg);

	// Send JOIN message to all other clients in channel

}

// :<nick>!<user>@<host> JOIN :<channel>
