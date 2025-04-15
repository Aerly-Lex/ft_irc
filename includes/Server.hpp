/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:09:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 14:48:37 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"
#include "Commands.hpp"

class User;
class Channel;

class Server
{
	private:
		int								_socket;
		int								_port;
		std::string						_password;
		std::vector<pollfd>				_fds;

		std::map<int, User>				_users;
		std::map<std::string, Channel>	_channels;
		Commands						_commands;

		std::chrono::system_clock::time_point	_creationTime;
		static bool	Signal;

	public:
		// Constructor, Destructor
		Server(int port, std::string password);
		~Server();

		// Getter
		int								getSocket() const;
		int								getPort() const;
		std::string						getPassword() const;
		std::string						getCreationTime() const;
		std::map<int, User>&			getUsers();
		std::map<std::string, Channel>&	getChannels();

		// Class methods
		static void		signalHandler(int signum);
		void			initServer();
		void			run();

		bool 	usersExists(std::string nickname);
		void	acceptNewUsers(std::vector<pollfd>& fds);
		void	removeUserFromAllChannels(int socket);
		void	handleUserMessage(std::vector<pollfd>& fds, int i);
		void	handleUserCommand(int clientSocket, const std::string& message);
		void	welcomeMsg(int userSocket);
		int		findTarget(const std::string &target);
};
