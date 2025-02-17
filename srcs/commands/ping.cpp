/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:33:06 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 12:44:02 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::ping(int userSocket)
{
	std::string	pongMsg = RPL_PONG(_users[userSocket]._nickName, "irc.server.com");
	std::cout << MAGENTA << pongMsg << std::endl << RESET;
	send(userSocket, pongMsg.c_str(), pongMsg.size(), 0);
}
