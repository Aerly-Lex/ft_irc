/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:10:27 by stopp             #+#    #+#             */
/*   Updated: 2025/04/23 13:37:42 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::who(int userSocket, std::string &channel)
{
	if (_channels.find(channel) != _channels.end())
	{
		sendTo(userSocket, RPL_NAMREPLY(_users[userSocket]._nickname, channel, _channels[channel].getNames()));
		sendTo(userSocket, RPL_ENDOFNAMES(_users[userSocket]._nickname, channel));
	}
}
