/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:11:41 by stopp             #+#    #+#             */
/*   Updated: 2025/04/07 18:30:07 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"


void	Commands::privmsg(int userSocket, std::string userMask, std::string channel, std::string message)
{
	_channels[channel].broadcast(userSocket, RPL_PRIVMSG(userMask, channel, message));
}
