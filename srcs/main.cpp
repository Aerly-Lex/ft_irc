/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:00:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 13:55:07 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

// !!!!RTFM!!!! !!!!RTFM!!!! !!!!RTFM!!!!
// https://beej.us/guide/bgnet/html/
// !!!!RTFM!!!! !!!!RTFM!!!! !!!!RTFM!!!!

// Function to validate and check if the port is within the acceptable range
// 0- 1023 are reserved ports
// 1024 - 49151 are registered and recommended ports to use
// 49152 - 65535 are dynamic or temporary ports
bool	checkPorts(char *argv, int *port)
{
	*port = std::atoi(argv);
	if (*port < 1024 || *port > 49151)
		return false;
	return true;
}

// > ./ircserv <port> <password>
// 	- port: The port number on which your IRC server will be listening to for incoming IRC connections.
// 	- password: The connection password. It will be needed by any IRC client that tries to connect to your server.
int	main(int argc, char **argv)
{
	int	port = 0;

	if (argc != 3)
		return (std::cerr << invalidArguments), 1;
	if (!checkPorts(argv[1], &port))
		return (std::cerr << invalidPort), 1;

	try
	{
		Server server(port, argv[2]); // save the port number and password

		server.initServer(); // initialize the server and set up the socket
		server.run(); // run the server and listen for incoming connections
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}

/*
toTest:
Memory leaks -> destructor im server.cpp

REQUIREMENTS:

ONLY FOR MACOS
Since MacOS doesn’t implement write() the same way as other Unix OSes, you are allowed to use fcntl().
You must use file descriptors in non-blocking mode in order to get a
behavior similar to the one of other Unix OSes.

TEST EXAMPLES
\$> nc -C 127.0.0.1 6667
com^Dman^Dd
*/
