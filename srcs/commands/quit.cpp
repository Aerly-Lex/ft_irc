/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:23:42 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/14 14:27:15 by Dscheffn         ###   ########.fr       */
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
