/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:45:09 by stopp             #+#    #+#             */
/*   Updated: 2025/04/09 12:41:18 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::kick(int userSocket, std::string channelName, std::string nickName, std::string reason)
{
	if (_channels[channelName].isOperator(userSocket) == false)
		sendTo(userSocket, ERR_CHANOPRIVSNEEDED(_users[userSocket]._nickname, channelName));
	else
	{
		int kickSocket = _channels[channelName].isMember(nickName);
		if (kickSocket > 0)
		{
			_channels[channelName].broadcast(0, ":" + _users[userSocket]._mask + " KICK " + channelName + " " + nickName + " " + reason + "\r\n");
			_channels[channelName].removeMember(kickSocket);
		}
		else
			sendTo(userSocket, ERR_USERNOTINCHANNEL(_users[userSocket]._nickname, nickName, channelName));
	}
}
