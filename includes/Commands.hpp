/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:31:17 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/18 15:27:19 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

class Server;
class User;
class Channel;

class Commands
{
	private:
		Server&								_server;
		std::map<int, User>&				_users;
		std::map< std::string, Channel>&	_channels;

	public:
		Commands(Server& server);
		~Commands();

		void	cap(int userSocket, const std::string& message);
		//invite
		void	join(int userSocket, const std::string& channelName);
		//mode
		void	nick(int userSocket, const std::string& newNickname);
		void	part(int userSocket, const std::string& channelName);
		void	ping(int userSocket);
		void	quit(int userSocket);
		//topic
};
