/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:46:18 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 11:49:45 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

Commands::Commands(Server& server)
: _server(server), _users(server.getUsers()), _channels(server.getChannels())
{

}

Commands::~Commands()
{

}
