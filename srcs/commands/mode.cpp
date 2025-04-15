/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Dscheffn <dscheffn@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:16:04 by stopp             #+#    #+#             */
/*   Updated: 2025/04/15 16:27:57 by Dscheffn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/includes.hpp"

void	Commands::mode(int userSocket, std::string channel, std::string flag, std::vector<std::string> params)
{
	if (flag.empty())
	{
		std::string modes = _channels[channel].getChannelModes();
		return sendTo(userSocket, RPL_CHANNELNAMEIS(_users[userSocket]._nickname, channel, modes));
	}
	if (flag == "b")
		return ;
	//checking for operator
	if (_channels[channel].isOperator(userSocket) == false)
		return sendTo(userSocket, ERR_CHANOPRIVSNEEDED(_users[userSocket]._nickname, channel));
	// now parsing so it works for complex mode commands
	char	sign, c;
	int		paramIdx = 0;
	int		limit;
	std::string nickname;

	for(int i = 0; i < flag.size(); i++)
	{
		c = flag[i];
		if (c == '+' || c == '-')
		{
			sign = c;
			continue;
		}

		switch (c)
		{
			case 'i': // Invite only mode
				if (sign == '+')
				{
					_channels[channel].setInviteOnly(true);
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " +i" + CRLF);
				}
				else
				{
					_channels[channel].setInviteOnly(false);
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " -i" + CRLF);
				}
				break ;
			case 't': //topic restriction for operator only
				if (sign == '+')
				{
					_channels[channel].setTopicRights(true);
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " +t" + CRLF);
				}
				else
				{
					_channels[channel].setTopicRights(false);
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " -t" + CRLF);
				}
				break;
			case 'k': // changing channel key (password)
				if (sign == '+')
				{
					if (paramIdx >= params.size())
					{
						sendTo(userSocket, ERR_INVALIDPARAM(_users[userSocket]._nickname, channel, "+k", "no password given"));
						break ;
					}
					_channels[channel].setPass(params[paramIdx]);
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " +k " + params[paramIdx++] + CRLF);
				}
				else
				{
					_channels[channel].setPass("");
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " -k" + CRLF);
				}
				break;
			case 'o': //changin operator privilidges
				if (paramIdx >= params.size())
				{
					sendTo(userSocket, ERR_INVALIDPARAM(_users[userSocket]._nickname, channel, "o", "no target given"));
					break ;
				}
				nickname = params[paramIdx++];
				if (sign == '+')
				{
					if (_channels[channel].isMember(nickname) == false)
						sendTo(userSocket, ERR_USERNOTINCHANNEL(_users[userSocket]._nickname, nickname, channel));
					else
					{
						if (_channels[channel].isOperator(_channels[channel].getSocket(nickname)) == true)
							break ;
						_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " +o " + nickname + CRLF);
						_channels[channel].addOperator(_channels[channel].getSocket(nickname), nickname);
					}
				}
				else
				{
					if (_channels[channel].isMember(nickname) == false)
						sendTo(userSocket, ERR_USERNOTINCHANNEL(_users[userSocket]._nickname, nickname, channel));
					else
					{
						if (_channels[channel].isOperator(_channels[channel].getSocket(nickname)) == false)
							break ;
						_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " -o " + nickname + CRLF);
						_channels[channel].removeOperator(_channels[channel].getSocket(nickname));
					}
				}
				break;
			case 'l': // changing userLimit of the channel
				if (sign == '+')
				{
					if (paramIdx >= params.size())
					{
						sendTo(userSocket, ERR_INVALIDPARAM(_users[userSocket]._nickname, channel, "+l", "no limit given"));
						break ;
					}
					try
					{
						limit = std::stoi(params[paramIdx]);
						_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " +l " + params[paramIdx++] + CRLF);
						_channels[channel].setUserLimits(limit);
					}
					catch(const std::exception& e)
					{
						sendTo(userSocket, ERR_INVALIDPARAM(_users[userSocket]._nickname, channel, "+l", "invalid limit"));
						break;
					}
				}
				else
				{
					_channels[channel].broadcast(0, ":" + _users[userSocket]._mask + " MODE " + channel + " l" + CRLF);
					_channels[channel].setUserLimits(0);
				}
				break;
		}
	}
}
