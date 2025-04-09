/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:31:17 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/09 16:03:16 by chorst           ###   ########.fr       */
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
		void	kick(int userSocket, std::string channelName, std::string nickName, std::string reason);
		void	invite(int inviterSocket, const std::string &targetNick, const std::string &channelName);
		void	privmsg(int userSocket, std::string userMask, std::string target, std::string message);
		//topic
};
