/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 10:04:15 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/07 15:21:26 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name)
{
	_topic = "";
	_password = "";
	_mode = "";
	inviteOnly = false;

}

Channel::~Channel()
{
}

std::string	Channel::getName() const
{
	return _name;
}
std::string	Channel::getTopic() const
{
	return _topic;
}
std::string	Channel::getPass() const
{
	return _password;
}
std::string	Channel::getMode() const
{
	return _mode;
}

void	Channel::setName(std::string &name)
{
	_name = name;
}
void	Channel::setMode(std::string &mode)
{
	_mode = mode;
}
void	Channel::setPass(std::string &password)
{
	_password = password;
}
void	Channel::setTopic(std::string &topic)
{
	_topic = topic;
}

bool	Channel::isBanned(const std::string &nick) const
{
	if (std::find(_banned.begin(), _banned.end(), nick) != _banned.end())
		return true;
	return false;
}

void	Channel::addMember(int userSocket, std::string &nick)
{
	if (_members.size() == 0 && _operators.size() == 0)
		_operators[userSocket] = nick;
	else if (_members.find(userSocket) == _members.end()
			&& isBanned(nick) == false)
		_members[userSocket] = nick;
	else
		return ;
}

void	Channel::removeMember(int userSocket)
{
	if (_members.find(userSocket) != _members.end())
	{
		_members[userSocket].erase();
		if (_operators.find(userSocket) != _members.end())
			_operators[userSocket].erase();
	}
}

void	Channel::banUser(std::string &nick)
{
	_banned.push_back(nick);
	for (auto it = _members.begin(); it != _members.end(); it++)
	{
		if (it->second == nick)
		{
			_members.erase(it);
			_operators.erase(it->first);
			it = _members.begin();
		}
	}
}

void	Channel::removeOperator(int userSocket)
{
	if (_operators.find(userSocket) == _operators.end())
		_operators[userSocket].erase();
}

void	Channel::addOperator(int userSocket, std::string &nick)
{
	if (_operators.find(userSocket) != _operators.end())
		_operators[userSocket] = nick;
}

std::string Channel::getNames() const
{
	std::string namelist;
	for(auto it = _members.begin(); it != _members.end(); it++)
	{
		if (_operators.find(it->first) != _operators.end())
			namelist += "@";
		namelist += it->second + " ";
	}
	return namelist;
}

// void	Channel::removeMember(int clientSocket)
// {
// 	_members.erase(std::remove(_members.begin(), _members.end(), clientSocket), _members.end());
// }

// void	Channel::broadcastMessage(int clientSocket, const std::string& message)
// {
// 	for (int member : _members)
// 	{
// 		if (member != clientSocket)
// 		{
// 			sendMessage(member, message);
// 		}
// 	}
// }
