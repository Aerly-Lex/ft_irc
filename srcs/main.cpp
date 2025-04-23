/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:00:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 16:43:46 by Dscheffn         ###   ########.fr       */
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
General ToDo:
im server.cpp:

Fix unknown command!
	Antwort von chorst: Ja, das ist ein Bug. Ich habs gefixed. Das war ein Fehler in der handleUserCommand() Funktion. Ich hab die Funktion so umgeschrieben, dass sie jetzt den richtigen Fehler zurückgibt. Die alte Funktion hab ich auskommentiert, falls die neue wieder Probleme macht.

Do we need WHO command?
	Antwort von chorst: Nein das ist nicht mandatory

There is NOTICE command?
	Antwort von chorst: Ja, aber nicht mandatory denn dafür gibt es "privmsg" im projekt (NOTICE ist sowas wie der kleine Bruder von privmsg)

User.cpp kann gelöscht werden I guess
	Anwort von chorst: Ja, kann es und ich habs grad getan

Doppelter Nickname in use - es kommt kein fehlermessage, dass nickname bereits vorhanden ist
	Antwort von chorst: Habs gefixed. Der ERR_NICKNAMEINUSE war falsch definiert und das ging vorher wegen dem fehlerhaften ERR_NICKNAMEINUSE-Makro nicht korrekt.

Christopher:
Extra funktion in utility packen, dass es whitespaces überspringen soll, entfernen soll keine ahnung, un die pos zuruckgeben
Goal: Ob im String nur noch spaces und \0 sind
Das ist notwendig im Server.cpp - server::handleusercommand, ganz unten
	if (!_users[userSocket]._buffer.empty())
	Antwort von chorst: Ja, das ist notwendig. Ich habs jetzt so gemacht, dass ich den String mit isOnlyWhitespace() überprüfe und dann die Funktion handleUserCommand() aufrufe. Das ist jetzt in der Server.cpp drin.

TEST EXAMPLES
\$> nc -C 127.0.0.1 6667
com^Dman^Dd
*/

// /* ************************************************************************** */
// /*	ICH HAB ALLE FUNKTIONEN GETESTET UND ALLES FUNKTIONIERT WIE ES SOLL (chorst)	*/
// /* **************************************************************************** */
