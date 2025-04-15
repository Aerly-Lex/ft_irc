/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 14:47:25 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

void	sendTo(int fd, const std::string &message)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Failed to send to fd: " << fd << std::endl;
}
