#pragma

#include "includes.hpp"

class Server {
	private:
		int			_socket;
		int			_port;
		std::string	_password;
		//	container for clients&channels
		// std::map< int, pollfd > _fds;
		std::vector<pollfd> _fds;

		static bool	Signal;

	public:
		// Constructor, Destructor
		Server(int port, std::string password);
		~Server();
		Server(Server const & src);
		Server & operator=(Server const & src);

		// Getter
		int				getSocket() const;
		int				getPort() const;
		std::string		getPassword() const;

		// Class methods
		static void		signalHandler(int signum);
		void			initServer();
		void			run();
};
