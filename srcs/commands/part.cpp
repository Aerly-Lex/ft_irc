/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:24:24 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/13 18:58:59 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// leave the channel

void Commands::part(int userSocket, const std::string& channelName)
{
	if (channelName.empty()) // not enough input args
		return sendTo(userSocket, ERR_NEEDMOREPARAMS(_users[userSocket]._nickname, "PART"));
	if (_channels.find(channelName) == _channels.end()) // channel does not exist
		return sendTo(userSocket, ERR_NOSUCHCHANNEL(_users[userSocket]._nickname, channelName));

	Channel& channel = _channels[channelName];

	if (!channel.isMember(userSocket))
		return sendTo(userSocket, ERR_NOTONCHANNEL(_users[userSocket]._nickname, channelName));

	std::string nickname = channel.getMembers()[userSocket];

	channel.broadcast(0, RPL_USERLEAVE(nickname, channelName));
	channel.removeMember(userSocket);

	if (channel.getMembers().empty())
		_channels.erase(channelName);
}

