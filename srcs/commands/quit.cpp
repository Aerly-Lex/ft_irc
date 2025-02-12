/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:23:42 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/12 12:37:39 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// disconnect from the server

void	Commands::quit(int clientSocket)
{
	// _userName <<
	std::cout << "Goodbye!" << std::endl;
	close(clientSocket);
}