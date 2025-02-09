/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 10:04:15 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/09 11:07:51 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

Channel::Channel(const std::string& name) : _name(name)
{
	_topic = "";
	_password = "";
	_mode = "";
	passwordProtected = false;
	inviteOnly = false;
}


void	Channel::addMember(int clientSocket)
{
	_members.push_back(clientSocket);
}

void	Channel::removeMember(int clientSocket)
{
	_members.erase(std::remove(_members.begin(), _members.end(), clientSocket), _members.end());
}

// void	Channel::broadcastMessage(int clientSocket, const std::string& message)
// {
// 	for (int member : _members)
// 	{
// 		if (member != clientSocket)
// 		{
// 			sendMessage(member, message);
// 		}
// 	}
// }
