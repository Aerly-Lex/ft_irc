/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweissha <aweissha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:03:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/24 17:10:18 by aweissha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

// trim whitespaces, strings etc. for the parsing

void customSend(int sockfd, std::string& string)
{
	ssize_t bytesSent = send(sockfd, string.c_str(), string.size(), 0);
	if (bytesSent == -1)
		throw std::runtime_error("Error sending message");
}
