/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:46:54 by stopp             #+#    #+#             */
/*   Updated: 2025/04/09 14:54:35 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::topic(int userSocket, std::string channel, std::string topic)
{
	if (_channels[channel].isOperator(userSocket) == false) //&& channel mode +t)
	{
		sendTo(userSocket, ERR_CHANOPRIVSNEEDED(_users[userSocket]._nickname, channel));
		return ;
	}
	else
	{
		
	}
}
