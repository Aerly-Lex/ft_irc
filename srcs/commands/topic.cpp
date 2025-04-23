/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:46:54 by stopp             #+#    #+#             */
/*   Updated: 2025/04/14 14:00:22 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// Topic command is for setting the topic of a channel.
void	Commands::topic(int userSocket, std::string channel, std::string topic)
{
	if (topic.empty())
	{
		if (_channels[channel].getTopic() == "")
			sendTo(userSocket, RPL_NOTOPIC(_users[userSocket]._nickname, channel));
		else
		{
			sendTo(userSocket, RPL_TOPIC(_users[userSocket]._nickname, channel, _channels[channel].getTopic()));
			sendTo(userSocket, RPL_TOPICWHOTIME(_users[userSocket]._nickname, channel, _channels[channel].getTopicData()));
		}
		return ;
	}
	if (_channels[channel].isTopicRights() == true && _channels[channel].isOperator(userSocket) == false)
		return sendTo(userSocket, ERR_CHANOPRIVSNEEDED(_users[userSocket]._nickname, channel));
	_channels[channel].broadcast(0, ":" + _users[userSocket]._nickname + " TOPIC " + channel + " :" + topic + "\r\n");
	_channels[channel].setTopic(topic);
	_channels[channel].setTopicData(_users[userSocket]._mask);
}
