/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:38:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/08 16:32:35 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

// https://modern.ircdocs.horse/
// Carriage Return Line Feed - for network communication
#define CRLF "\r\n"

#define SRV_NAME "IRC_GANG"

#define RPL_WELCOME(nick) (":" + std::string(SRV_NAME) + " 001 " + nick + " :The 42 IRC server project presented by Sören, Chris & Daniel" + CRLF)

// !!!002 (RPL_YOURHOST) - info about server
#define RPL_YOURHOST(nick) (":" + std::string(SRV_NAME) + " 002 " + nick + " :Your host is " + std::string(SRV_NAME) + " running version 1.0" + CRLF)
// !!!003 (RPL_CREATED) - server creation date
#define RPL_CREATED(nick, created) (":" + std::string(SRV_NAME) + " 003 " + std::string(nick) + " :This server was created at: " + std::string(created) + CRLF)
// !!!004 (RPL_MYINFO) - server detail

// 004 - Server details
// #define RPL_MYINFO(nick) (":" std::string(SRV_NAME) + " 004 " + nick + " :")
// std::string myInfo = ":irc.server.com 004 " + _users[userSocket]._nickname + " irc.server.com 1.0 iov" + CRLF;

// // send PONG on incoming PING
#define RPL_PONG(nickname, hostname) ("PONG" + nickname + " " + hostname + CRLF)

// RPL_JOINMSG replies the Joinmessage of the client as confirmation
#define RPL_JOINMSG(nickname, username, hostname, channel) (":" + nickname + "!" + username + "@" + hostname + " JOIN " + channel + CRLF)
// RPL_TOPIC (332) responds with the topic of the channel when there is one
#define RPL_TOPIC(nickname, channelname, topic) (":" + std::string(SRV_NAME) + " 332 " + nickname + " " + channelname + " :" + topic + CRLF)
// RPL_NAMREPLY (353) sends a list of all members of the channel
#define RPL_NAMREPLY(nickname, channelname, names) (":" + std::string(SRV_NAME) + " 353 " + nickname + " = " + channelname + " :" + names + CRLF)
// RPL_ENDOFNAMES (366) Ends member list
#define RPL_ENDOFNAMES(nickname, channelname) (":" + std::string(SRV_NAME) + " 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)
// handles the message coming from a user
#define RPL_PRIVMSG(userMask, channel, message) (":" + userMask + " PRIVMSG " + channel + " " + message + CRLF)

// NICKnames Stuff
// NICK <username> - change nickname
#define RPL_NICKCHANGE(oldnickname, nickname) (std::string(":" + oldnickname + " NICK " + nickname + CRLF))
#define ERR_NONICKNAMEGIVEN() (":" + std::string(SRV_NAME) + " 431 * :No nickname given" + CRLF)
#define ERR_NOTREGISTERED() (":" + std::string(SRV_NAME) + " 451 * :You have not registered" + CRLF)
#define ERR_NICKNAMEINUSE(nickname) (std::string(":" + std::string(SRV_NAME) + " 433 " + nickname + " :Nickname is already in use" + CRLF))
// there is a max len for nick appearantly... 9 chars - didnt test yet
// 401 - NOSUCHNICK  if there is no such nickname or channel
#define ERR_NOSUCHNICK(nickname, target) (":" + std::string(SRV_NAME) + " 401 " + nickname + target + " :No such nick/channel" + CRLF)

// cleaned comments, fixed small issue with nick message
