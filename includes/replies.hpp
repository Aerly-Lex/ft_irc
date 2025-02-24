/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweissha <aweissha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:38:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/24 17:30:07 by aweissha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

// https://modern.ircdocs.horse/
// Carriage Return Line Feed - for network communication
#define CRLF "\r\n"

// Welcome Messages
#define RPL_WELCOME(source) (": 001 " + source + " :Welcome to the 42 IRC server project presented by Alex & Daniel" + CRLF)
#define RPL_YOURHOST(source, servername, version) (": 002 " + source + " :Your host is " + servername + ", running version " + version + CRLF)
#define RPL_CREATED(source, datetime) (": 003 " + source + " :This server was created " + datetime + CRLF)
#define RPL_MYINFO(source, servername, version, usermodes, chanmodes) (": 004 " + source + " " + servername + " " + version + " " + usermodes + " " + chanmodes + CRLF)

// PING/PONG Handling
#define RPL_PONG(nickname, hostname) (":PONG " + nickname + " " + hostname + CRLF)

// Channel Replies
#define RPL_JOINMSG(nickname, username, hostname, channel) (":" + nickname + "!" + username + "@" + hostname + " JOIN " + channel + CRLF)
#define RPL_NAMREPLY(source, symbol, channel, names) (": 353 " + source + " " + symbol + " " + channel + " :" + names + CRLF)
#define RPL_ENDOFNAMES(source, channel) (": 366 " + source + " " + channel + " :End of /NAMES list" + CRLF)

// Nickname Handling
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + CRLF)
#define ERR_NONICKNAMEGIVEN (": 431 :No nickname given" + CRLF)
#define ERR_NICKNAMEINUSE(source) (": 433 * " + source + " :Nickname is already in use" + CRLF)
#define ERR_ERRONEUSNICKNAME(source) (": 432 " + source + " :Erroneous nickname" + CRLF)
#define ERR_NOTREGISTERED(source) (": 451 " + source + " :You have not registered" + CRLF)

// User Authentication
#define ERR_NEEDMOREPARAMS(command) (": 461 " + command + " :Not enough parameters" + CRLF)
#define ERR_ALREADYREGISTERED(source) (": 462 " + source + " :You may not reregister" + CRLF)
#define RPL_YOUREOPER(source) (": 381 " + source + " :You are now an IRC operator" + CRLF)

// Private Messaging
#define RPL_PRIVMSG(sender, receiver, message) (":" + sender + " PRIVMSG " + receiver + " :" + message + CRLF)
#define ERR_NORECIPIENT(command) (": 411 :No recipient given (" + command + ")" + CRLF)
#define ERR_NOTEXTTOSEND (": 412 :No text to send" + CRLF)
#define ERR_CANNOTSENDTOCHAN(channel) (": 404 " + channel + " :Cannot send to channel" + CRLF)

// Errors
#define ERR_UNKNOWNCOMMAND(command) (": 421 " + command + " :Unknown command" + CRLF)
#define ERR_TOOMANYCHANNELS(source, channel) (": 405 " + source + " " + channel + " :You have joined too many channels" + CRLF)
#define ERR_NOSUCHNICK(source) (": 401 " + source + " :No such nick/channel" + CRLF)
#define ERR_NOSUCHCHANNEL(channel) (": 403 " + channel + " :No such channel" + CRLF)
#define ERR_USERNOTINCHANNEL(source, channel) (": 441 " + source + " " + channel + " :They aren't on that channel" + CRLF)
#define ERR_NOTONCHANNEL(channel) (": 442 " + channel + " :You're not on that channel" + CRLF)
#define ERR_BADCHANNELKEY(source, channel) (": 475 " + source + " " + channel + " :Cannot join channel (+k)" + CRLF)
#define ERR_CHANNELISFULL(source, channel) (": 471 " + source + " " + channel + " :Cannot join channel (+l)" + CRLF)
#define ERR_PASSWDMISMATCH(source) (": 464 " + source + " :Password is incorrect" + CRLF)

// command responses
#define RPL_JOIN(source, channel) (":" + source + " JOIN :" + channel + CRLF)
#define RPL_PING(source, token) (":" + source + " PONG :" + token + CRLF)
#define RPL_KICK(source, channel, target, reason) (":" + source + " KICK " + channel + " " + target + " :" + reason + CRLF)
#define RPL_PART(source, channel) (":" + source + " PART :" + channel + CRLF)
#define RPL_MODE(source, channel, modes, args) (":" + source + " MODE " + channel + " " + modes + " " + args + CRLF)
#define RPL_QUIT(source, message) (":" + source + " QUIT :Quit: " + message + CRLF)
#define RPL_NOTICE(source, target, message) (":" + source + " NOTICE " + target + " :" + message + CRLF)





// #pragma once

// #include "includes.hpp"

// // https://modern.ircdocs.horse/
// // Carriage Return Line Feed - for network communication
// #define CRLF "\r\n"

// #define RPL_WELCOME(nick) (": 001" + nick + " :The 42 IRC server project presented by Alex & Daniel" + CRLF)

// // !!!002 (RPL_YOURHOST) - info about server
// // !!!003 (RPL_CREATED) - server creation date
// // !!!004 (RPL_MYINFO) - server detail

// // // send PONG on incoming PING
// #define RPL_PONG(nickname, hostname) ("PONG" + nickname + " " + hostname + CRLF)

// // RPL_NAMREPLY (353) ??
// //Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.
// #define RPL_JOINMSG(nickname, username, hostname, channel) (std::string(":") + nickname + "!" + username + "@" + hostname + " JOIN " + channel + CRLF)
// //RPL_ENDOFNAMES (366) ??
// //   "<client> <channel> :End of /NAMES list"
// // Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.


// // NICKnames Stuff
// // NICK <username> - change nickname
// #define RPL_NICKCHANGE(oldnickname, nickname) (std::string(":" + oldnickname + " NICK " + nickname + CRLF))
// // !!!ERR_NONICKNAMEGIVEN
// // !!!ERR_NOTREGISTERED
// // !!!ERR_NICKNAMEINUSE
// // there is a max len for nick appearantly... 9 chars - didnt test yet

// // cleaned comments, fixed small issue with nick message