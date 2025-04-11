/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:22:16 by chorst            #+#    #+#             */
/*   Updated: 2025/04/11 16:29:16 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void Commands::invite(int inviterSocket, std::string &targetNick, const std::string &channelName)
{
	if (_channels[channelName].isOperator(inviterSocket) == false)
		return sendTo(inviterSocket, ERR_CHANOPRIVSNEEDED(_users[inviterSocket]._nickname, targetNick));

	int targetSocket = -1;
	for (auto it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second._nickname == targetNick)
		{
			targetSocket = it->first;
			break ;
		}
	}
	if (targetSocket == -1)
		return sendTo(inviterSocket, ERR_NOSUCHNICK(_users[inviterSocket]._nickname, targetNick));

	if (_channels[channelName].isMember(targetSocket))
		return sendTo(inviterSocket, ERR_USERONCHANNEL(_users[inviterSocket]._nickname ,targetNick, channelName));
	sendTo(inviterSocket, RPL_INVITING(_users[inviterSocket]._nickname, targetNick, channelName));
	sendTo(targetSocket, (":" + _users[inviterSocket]._mask + " INVITE " + targetNick + " " + channelName + "\r\n"));
	_channels[channelName].addInvited(targetNick);
}
