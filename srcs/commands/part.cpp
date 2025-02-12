/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:24:24 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/12 13:38:06 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// leave the channel

void	Commands::part(int clientSocket, const std::string& channelName)
{
	// iss >> channelName;
	_server.getChannels()[channelName].removeMember(clientSocket);
	std::cout << "You have left " << channelName << std::endl;
	// does channel exist?
	// need password?
	// need invite? (check mode)
	// _userName <<
	// remove the client from the channel
	// _channels[channelName].removeMember(clientSocket);
}