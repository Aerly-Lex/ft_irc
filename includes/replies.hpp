/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:38:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/15 10:42:22 by chorst           ###   ########.fr       */
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
// RPL_CHANNELMODEIS (324) sending the modes of the Channel
#define RPL_CHANNELNAMEIS(nickname, channelname, modes) (":" + std::string(SRV_NAME) + " 324 " + nickname + " " + channelname + " " + modes + CRLF)
// RPL_JOINMSG replies the Joinmessage of the client as confirmation
#define RPL_JOINMSG(nickname, username, hostname, channel) (":" + nickname + "!" + username + "@" + hostname + " JOIN " + channel + CRLF)
// RPL_NOTOPIC (331) return message if no topic is set
#define RPL_NOTOPIC(nickname, channelname) (":" + std::string(SRV_NAME) + " 331 " + nickname + " " + channelname + " :No topic set!" + CRLF)
// RPL_TOPIC (332) replies with the Topic of the Channel
#define RPL_TOPIC(nickname, channelname, topic) (":" + std::string(SRV_NAME) + " 332 " + nickname + " " + channelname + " :" + topic + CRLF)
// RPL_TOPICWHOTIME (333) replies with metadata of topic
#define RPL_TOPICWHOTIME(nickname, channelname, topicData) (":" + std::string(SRV_NAME) + " 333 " + nickname + " " + channelname + " " + topicData + CRLF)
// RPL_TOPIC (332) responds with the topic of the channel when there is one
#define RPL_TOPIC(nickname, channelname, topic) (":" + std::string(SRV_NAME) + " 332 " + nickname + " " + channelname + " :" + topic + CRLF)
// RPL_INVITING (341)
#define RPL_INVITING(opname, nickname, channelname) (":" + std::string(SRV_NAME) + " 341 " + opname + " " + nickname + " " + channelname + CRLF)
// RPL_NAMREPLY (353) sends a list of all members of the channel
#define RPL_NAMREPLY(nickname, channelname, names) (":" + std::string(SRV_NAME) + " 353 " + nickname + " = " + channelname + " :" + names + CRLF)
// RPL_ENDOFNAMES (366) Ends member list
#define RPL_ENDOFNAMES(nickname, channelname) (":" + std::string(SRV_NAME) + " 366 " + nickname + " " + channelname + " :END of /NAMES list" + CRLF)
// handles the message coming from a user
#define RPL_PRIVMSG(userMask, channel, message) (":" + userMask + " PRIVMSG " + channel + " " + message + CRLF)
// handles the message of a leaving user from a channel
#define RPL_USERLEAVE(nickname, channelname) (":" + nickname + "!user@localhost PART " + channelname + " :" + nickname + " has left the channel" + CRLF)


// NICKnames Stuff
// NICK <username> - change nickname
#define RPL_NICKCHANGE(oldnickname, nickname) (std::string(":" + oldnickname + " NICK " + nickname + CRLF))
#define ERR_NONICKNAMEGIVEN() (":" + std::string(SRV_NAME) + " 431 * :No nickname given" + CRLF)
#define ERR_NOTREGISTERED() (":" + std::string(SRV_NAME) + " 451 * :You have not registered" + CRLF)
#define ERR_NICKNAMEINUSE(nickname) (std::string(":" + std::string(SRV_NAME) + " 433 " + nickname + " :Nickname is already in use" + CRLF))
// there is a max len for nick appearantly... 9 chars - didnt test yet
// 401 - NOSUCHNICK  if there is no such nickname or channel
#define ERR_NOSUCHNICK(nickname, target) (":" + std::string(SRV_NAME) + " 401 " + nickname + target + " :No such nick/channel" + CRLF)
// ERR_NOSUCHCHANNEL (403)
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + std::string(SRV_NAME) + " 403 " + nick + " " + channel + " :No such channel" + CRLF)

// ERR_UNKNOWNCOMMAND (421)
#define ERR_UNKNOWNCOMMAND(nickname, command) (":" + std::string(SRV_NAME) + " 421 " + nickname + " " + command + " :Unknown command" + CRLF)
// ERR_USERNOTINCHANNEL (441)
#define ERR_USERNOTINCHANNEL(opname, nickname, channelname) (":" + std::string(SRV_NAME) + " 441 " + opname + " " + nickname + " " + channelname + " :User not in the Channel" + CRLF)
// ERR_NOTONCHANNEL (442)
#define ERR_NOTONCHANNEL(nick, channel) (":" + std::string(SRV_NAME) + " 442 " + nick + " " + channel + " :You're not on that channel" + CRLF)
// ERR_USERONCHANNEL (443)
#define ERR_USERONCHANNEL(opname, nickname, channelname) (":" + std::string(SRV_NAME) + " 442 " + opname + " " + nickname + " " + channelname + " :User is already on channel" + CRLF)
// ERR_NEEDMOREPARAMS (461)
#define ERR_NEEDMOREPARAMS(nickname, command) (":" + std::string(SRV_NAME) + " 461 " + nickname + " " + command + " :Not enough parameters" + CRLF)
// ERR_ALREADYREGISTRED (464)
#define ERR_PASSWDMISMATCH(nickname) (":" + std::string(SRV_NAME) + " 464 " + nickname + " :Password incorrect" + CRLF)
// ERR_INVITEONLYCHAN (473)
#define ERR_INVITEONLYCHAN(nickname, channelname) (":" + std::string(SRV_NAME) + " 473 " + nickname + " " + channelname + " :Cannot join channel" + CRLF)
// ERR_BADCHANNELKEY (475) if given wrong or no password for protected channel
#define ERR_BADCHANNELKEY(nickname, channelname) (":" + std::string(SRV_NAME) + " 475 " + nickname + " " + channelname + " :Cannot join Channel (+k)" + CRLF)
// ERR_CHANOPRIVSNEEDED (482) - no operator priviledges for the user
#define ERR_CHANOPRIVSNEEDED(nickname, channelname) (":" + std::string(SRV_NAME) + " 482 " + nickname + " " + channelname + " :You're not a channel operator" + CRLF)
// ERR_INVALIDPARAM (696)
#define ERR_INVALIDPARAM(nickname, channelname, modechar, reason) (":" + std::string(SRV_NAME) + " 696 " + nickname + " " + channelname + " " + modechar + " :" + reason + CRLF)
// cleaned comments, fixed small issue with nick message
