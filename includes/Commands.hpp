/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:31:17 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/12 13:10:44 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "includes.hpp"

class Server;

class Commands
{
	private:
		Server&	_server;

	public:
		Commands(Server& server);
		~Commands();

		void	part(int clientSocket, const std::string& channelName);
		void	quit(int clientSocket);
};
