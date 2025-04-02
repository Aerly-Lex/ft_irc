/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:09:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/02 13:00:11 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"
#include "Commands.hpp"

class User;
class Channel;
// class Commands;

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

		static bool	Signal;

	public:
		// Constructor, Destructor
		Server(int port, std::string password);
		~Server();

		// Getter
		int								getSocket() const;
		int								getPort() const;
		std::string						getPassword() const;
		std::map<int, User>&			getUsers();
		std::map<std::string, Channel>&	getChannels();

		// Class methods
		static void		signalHandler(int signum);
		void			initServer();
		void			run();

		void	acceptNewUsers(std::vector<pollfd>& fds);
		void	handleUserMessage(std::vector<pollfd>& fds, int i);
		void	handleUserCommand(int clientSocket, const std::string& message);
		void	sendTo(int fd, const std::string &message);
};
