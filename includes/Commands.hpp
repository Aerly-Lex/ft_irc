/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:31:17 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/17 10:33:28 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

class Server;
class Client;
class Channel;

class Commands
{
	private:
		Server&								_server;
		std::map<int, Client>&				_clients;
		std::map< std::string, Channel>&	_channels;

	public:
		Commands(Server& server);
		~Commands();

		void	part(int clientSocket, const std::string& channelName);
		void	quit(int clientSocket);
		void	join(int clientSocket, const std::string& channelName);
		void	nick(int clientSocket, const std::string&nickName);
};
