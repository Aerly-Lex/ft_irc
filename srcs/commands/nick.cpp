/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:26:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 12:40:00 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::nick(int userSocket, const std::string&nickName)
{
	_users[userSocket]._nickName = nickName;

	std::cout << "User " << userSocket << " set nickname to: " << nickName << std::endl;

	//check if nickname is already in use

	//send NICK changes message to client
	// if (!_users[userSocket]._username.empty())

}