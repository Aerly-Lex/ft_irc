/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:23:42 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/12 17:49:59 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// disconnect from the server

void	Commands::quit(int userSocket)
{
	for (auto it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second.isMember(_users[userSocket]._nickname))
			it->second.broadcast(userSocket, ":" + _users[userSocket]._mask + " QUIT :LEAVING" + CRLF);
	}
	sendTo(userSocket, ":" + _users[userSocket]._nickname + " ERROR :Closing connection" + CRLF);
	std::cout << "Goodbye!" << std::endl;
	shutdown(userSocket, SHUT_RDWR);
	close(userSocket);
	_server.removeUserFromAllChannels(userSocket);
	_users.erase(userSocket);
}
