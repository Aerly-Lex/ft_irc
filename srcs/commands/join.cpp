/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:20 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/07 15:00:23 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::join(int userSocket, const std::string& channelName)
{
	// Check if channel exists, if not create it
	if (_channels.find(channelName) == _channels.end())
	{
		Channel	newChannel(channelName);
		_channels[channelName] = newChannel;
	}
	// Add client to channel
	_channels[channelName].addMember(userSocket, _users[userSocket]._nickname);

	// Send JOIN message to client
	// std::string	joinMsg = ":" + _clients[clientSocket]._nickName + " JOIN :" + channelName + CRLF;
	std::cout << _users[userSocket]._nickname + "!" + _users[userSocket]._userName + "@" + _users[userSocket]._hostName + " JOIN :" + channelName << std::endl;
	std::string joinMsg = RPL_JOINMSG(_users[userSocket]._nickname, _users[userSocket]._userName, _users[userSocket]._hostName, channelName);
	std::cout << RED << joinMsg << std::endl << RESET;
	_server.sendTo(userSocket, joinMsg);
	if (_channels[channelName].getTopic() != "")
	{
		joinMsg = RPL_TOPIC(_users[userSocket]._nickname, channelName, _channels[channelName].getTopic());
		_server.sendTo(userSocket, joinMsg);
		std::cout << RED << joinMsg << std::endl << RESET;
	}
	joinMsg = RPL_NAMREPLY(_users[userSocket]._nickname, channelName, _channels[channelName].getNames());
	_server.sendTo(userSocket, joinMsg);
	std::cout << RED << joinMsg << std::endl << RESET;
	// Send JOIN message to all other clients in channel

}

// :<nick>!<user>@<host> JOIN :<channel>
