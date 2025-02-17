/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:26:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 10:31:03 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::nick(int clientSocket, const std::string&nickName)
{
	_clients[clientSocket]._nickName = nickName;

	std::cout << "Client " << clientSocket << " set nickname to: " << nickName << std::endl;

	//check if nickname is already in use

	//send NICK changes message to client
	// if (!_clients[clientSocket]._username.empty())

}