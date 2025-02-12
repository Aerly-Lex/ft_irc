/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:09:28 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/12 13:11:02 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"
#include "Commands.hpp"

class Client;
class Channel;
// class Commands;

class Server
{
	private:
		int								_socket;
		int								_port;
		std::string						_password;
		std::vector<pollfd>				_fds;

		std::map<int, Client>			_clients;
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
		std::map<int, Client>&			getClients();
		std::map<std::string, Channel>&	getChannels();

		// Class methods
		static void		signalHandler(int signum);
		void			initServer();
		void			run();

		void	acceptNewCients(std::vector<pollfd>& fds);
		void	handleClientMessage(std::vector<pollfd>& fds, int i);
		void	handleClientCommand(int clientSocket, const std::string& message);
};
