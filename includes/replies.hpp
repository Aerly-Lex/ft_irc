/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:38:50 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/02/16 11:29:52 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"

// https://modern.ircdocs.horse/
// Carriage Return Line Feed - for network communication
#define CRLF "\r\n"

#define RPL_WELCOME(nick) (": 001" + nick + " :The 42 IRC server project presented by Alex & Daniel" + CRLF)

// RPL_NAMREPLY (353)
//Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.
#define RPL_JOINMSG(nickname, username, hostname, channel) (std::string(":") + nickname + "!" + username + "@" + hostname + " JOIN " + channel + CRLF)
//RPL_ENDOFNAMES (366)
//   "<client> <channel> :End of /NAMES list"
// Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.
