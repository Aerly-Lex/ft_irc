/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:00:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/19 15:08:40 by Dscheffn         ###   ########.fr       */
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
	if (!checkPorts(argv[1], &port) )
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
Daniel ToDo:
implement CAP
slim handleUserMessage function! too many repetitives...


Alex ToDo:
implement own send function and replace everywhere, fuck this typting c_str(), size everytime - perhaps as a method in server class
implement the replies (commented in includes/replies.hpp) (NICK, JOIN, PING, etc.)), everthing with 3x!!!


ALLOWED FUNCTIONS, among others:
setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, connect, accept, htons,
htonl, ntohs, ntohl, inet_addr, inet_ntoa,
sigaction, lseek, fstat, fcntl,
USED:
socket, close, bind, listen, poll, recv, signal, send,

REQUIREMENTS:
> Server must handle multiple clients at the same time and never hang

> Forking is not allowed. All I/O operations must be non-blocking.

> Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
Because you have to use non-blocking file descriptors, it is possible to use read/recv or write/send functions with no poll()
(or equivalent), and your server wouldn’t be blocking.
But it would consume more system resources.
Thus, if you try to read/recv or write/send in any file descriptor without using poll() (or equivalent), your grade will be 0.

> Communication between client and server has to be done via TCP/IP (v4 or v6).

> Using your reference client with your server must be similar to using it with any official IRC server. However, you only have to implement the following features:
> You must be able to	authenticate
						set a nickname
						a username
						join a channel
						send and receive private messages using your reference client.
> All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
> You must have operators and regular users.

> CHANNEL OPERATOR COMMANDS:
	∗ KICK - Eject a client from the channel
	∗ INVITE - Invite a client to a channel
	∗ TOPIC - Change or view the channel topic
	∗ MODE - Change the channel’s mode:
		- i: Set/remove Invite-only channel
		- t: Set/remove the restrictions of the TOPIC command to channel operators
		- k: Set/remove the channel key (password)
		- o: Give/take channel operator privilege
		- l: Set/remove the user limit to channel


ONLY FOR MACOS
Since MacOS doesn’t implement write() the same way as other Unix OSes, you are allowed to use fcntl().
You must use file descriptors in non-blocking mode in order to get a
behavior similar to the one of other Unix OSes.

However, you are allowed to use fcntl() only as follows:
fcntl(fd, F_SETFL, O_NONBLOCK);
Any other flag is forbidden.


TEST EXAMPLES
Verify absolutely every possible error and issue (receiving partial data, low bandwidth, and so forth).
To ensure that your server correctly processes everything that you send to it, the
following simple test using nc can be done:
\$> nc -C 127.0.0.1 6667
com^Dman^Dd
\$>
Use ctrl+D to send the command in several parts: ’com’, then ’man’, then ’d\n’.
In order to process a command, you have to first aggregate the received packets in
order to rebuild it.
*/