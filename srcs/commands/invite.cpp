/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:22:16 by chorst            #+#    #+#             */
/*   Updated: 2025/04/09 16:12:05 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void Commands::invite(int inviterSocket, const std::string &targetNick, const std::string &channelName)
{
	if(!_users.count(inviterSocket)) // Is inviterUser existend?
		return sendTo(inviterSocket, ERR_NOTREGISTERED());

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

}
