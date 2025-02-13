/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:20 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/13 14:27:20 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::join(int clientSocket, const std::string& channelName)
{
	// Check if channel exists, if not create it
	if (_server.getChannels().find(channelName) == _server.getChannels().end())
	{
		Channel	newChannel(channelName);
		newChannel._name = channelName;
		_server.getChannels()[channelName] = newChannel;
	}
	// Add client to channel
	_server.getChannels()[channelName].members.push_back(clientSocket);

	// Send JOIN message to client
	std::string	joinMsg = ":" + _clients[clientSocket]._nickName + " JOIN :" + channelName + CRLF;
	std::cout << RED << joinMsg << std::endl << RESET;
	send(clientSocket, joinMsg.c_str(), joinMsg.size(), 0);

}