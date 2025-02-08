#pragma once

#include "includes.hpp"

class Client;

class Server {
	private:
		int			_socket;
		int			_port;
		std::string	_password;
		std::vector<pollfd> _fds;

		std::map<int, Client> _clients;
		//container for channel?

		static bool	Signal;

	public:
		// Constructor, Destructor
		Server(int port, std::string password);
		~Server();

		// Getter
		int				getSocket() const;
		int				getPort() const;
		std::string		getPassword() const;

		// Class methods
		static void		signalHandler(int signum);
		void			initServer();
		void			run();

		void	acceptNewCients(std::vector<pollfd>& fds);
		void	handleClientMessage(std::vector<pollfd>& fds, int i);
};
