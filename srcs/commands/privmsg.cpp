/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:11:41 by stopp             #+#    #+#             */
/*   Updated: 2025/04/23 15:34:16 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

// send a private message to a user or everyone in a channel
void	Commands::privmsg(int userSocket, std::string userMask, std::string target, std::string message)
{
	int	tgt = _server.findTarget(target);
	if (tgt == 0)
		sendTo(userSocket, ERR_NOSUCHNICK(_users[userSocket]._nickname, target));
	else if (tgt < 0)
		_channels[target].broadcast(userSocket, RPL_PRIVMSG(userMask, target, message));
	else
		sendTo(tgt, RPL_PRIVMSG(userMask, target, message));
}
