/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:46:18 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/13 14:02:48 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

Commands::Commands(Server& server)
: _server(server), _clients(server.getClients()), _channels(server.getChannels())
{

}

Commands::~Commands()
{

}
