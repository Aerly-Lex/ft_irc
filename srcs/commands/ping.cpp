/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:33:06 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/02 13:10:16 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::ping(int userSocket)
{
	std::string	pongMsg = RPL_PONG(_users[userSocket]._nickname, "irc.server.com");
	std::cout << MAGENTA << pongMsg << std::endl << RESET;
	_server.sendTo(userSocket, pongMsg);
}
