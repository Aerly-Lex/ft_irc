/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:20 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 16:41:16 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::join(int userSocket, const std::string& channelName, std::string &password)
{
	// Check if channel exists, if not create it
	if (_channels.find(channelName) == _channels.end())
	{
		Channel	newChannel(channelName);
		_channels[channelName] = newChannel;
	}

	// check for invite only and if invited
	if (_channels[channelName].isInviteOnly() && !_channels[channelName].isInvited(_users[userSocket]._nickname))
		return sendTo(userSocket, ERR_INVITEONLYCHAN(_users[userSocket]._nickname, channelName));

	// check for password
	if (_channels[channelName].getPass() != password)
		return sendTo(userSocket, ERR_BADCHANNELKEY(_users[userSocket]._nickname, channelName));

	// Add client to channel
	_channels[channelName].addMember(userSocket, _users[userSocket]._nickname);

	if (_channels[channelName].isInvited(_users[userSocket]._nickname))
		_channels[channelName].removeInvited(_users[userSocket]._nickname);

	// Send JOIN message to client
	std::cout << _users[userSocket]._nickname + "!" + _users[userSocket]._userName + "@" + _users[userSocket]._hostName + " JOIN :" + channelName << std::endl;
	std::string	joinMsg = RPL_JOINMSG(_users[userSocket]._nickname, _users[userSocket]._userName, _users[userSocket]._hostName, channelName);
	std::cout << RED << joinMsg << std::endl << RESET;
	sendTo(userSocket, joinMsg);

	std::string	names;
	if (_channels[channelName].getTopic() != "")
	{
		names = RPL_TOPIC(_users[userSocket]._nickname, channelName, _channels[channelName].getTopic());
		sendTo(userSocket, names);
		std::cout << RED << names << std::endl << RESET;
	}
	names = RPL_NAMREPLY(_users[userSocket]._nickname, channelName, _channels[channelName].getNames());
	sendTo(userSocket, names);
	std::cout << RED << names << std::endl << RESET;
	names = RPL_ENDOFNAMES(_users[userSocket]._nickname, channelName);
	sendTo(userSocket, names);
	std::cout << RED << names << std::endl << RESET;

	// Send JOIN message to all other clients in channel
	_channels[channelName].broadcast(userSocket, joinMsg);
}

// :<nick>!<user>@<host> JOIN :<channel>
