/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorst <chorst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 10:04:15 by Dscheffn          #+#    #+#             */
/*   Updated: 2025/04/14 14:09:52 by chorst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/includes.hpp"

//////////////////////////////////////////
//				Constructor				//
//////////////////////////////////////////

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name)
{
	_topic = "";
	_password = "";
	_userLimit = 0;
	_inviteOnly = false;
	_topic_rigths = false;
}

Channel::~Channel() {}

//////////////////////////////////////////
//				getter					//
//////////////////////////////////////////

std::string					Channel::getName() const { return _name; }
std::string					Channel::getTopic() const { return _topic; }
std::string					Channel::getTopicData() const { return _topicData; }
std::string					Channel::getPass() const{ return _password; }
int							Channel::getUserLimit() const { return _userLimit; }
bool						Channel::isInviteOnly() const{ return _inviteOnly; }
bool						Channel::isTopicRights() const { return _topic_rigths; }
std::map<int, std::string>&	Channel::getMembers() { return _members; }

int	Channel::getSocket(std::string nickname)
{
	for (auto it = _members.begin(); it != _members.end(); it++)
		if (it->second == nickname)
			return (it->first);
	return (0);
}

std::string		Channel::getChannelModes() const
{
	std::string modes = "+";
	if (!_password.empty())
		modes += "k";
	if (_inviteOnly == true)
		modes += "i";
	if (_topic_rigths == true)
		modes += "t";
	if (_userLimit > 0)
	{
		modes += "l";
		modes += " " + std::to_string(_userLimit);
	}
	return modes;
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

bool	Channel::isInvited(std::string &user) const
{
	if (_invited.count(user) > 0)
		return true;
	return false;
}

//////////////////////////////////////////
//				setter					//
//////////////////////////////////////////

void	Channel::setName(std::string &name) { _name = name; }
void	Channel::setTopic(std::string &topic) { _topic = topic; }
void	Channel::setPass(std::string password) { _password = password; }
void	Channel::setUserLimits(int userLimit) { _userLimit = userLimit; }
void	Channel::setInviteOnly(bool mode) { _inviteOnly = mode; }
void	Channel::setTopicRights(bool rights) { _topic_rigths = rights; }

void	Channel::setTopicData(std::string &userMask)
{
	_topicData = std::string(userMask + " " + std::to_string(std::time(nullptr)));
}

//////////////////////////////////////////
//				class methods			//
//////////////////////////////////////////

void	Channel::addInvited(std::string &nickname)
{
	_invited.insert(nickname);
}

void	Channel::removeInvited(std::string nickname)
{
	_invited.erase(nickname);
}

bool Channel::memberExists(const std::string &nick) const // checks if a nick is already in the _members map
{
	for (std::map<int, std::string>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		if (it->second == nick)
			return true;
	return false;
}

bool	Channel::isOperator(int userSocket) const
{
	if (_operators.find(userSocket) == _operators.end())
		return false;
	return true;
}

bool Channel::isMember(int socket) const
{
	return _members.count(socket) > 0;
}

int		Channel::isMember(std::string nickName)
{
	auto it = _members.begin();
	for (; it != _members.end(); it++)
	{
		if(it->second == nickName)
			return (it->first);
	}
	return (0);
}

void Channel::addMember(int userSocket, const std::string &nick) // adds a user to the _members map
{
	if (_members.count(userSocket) || memberExists(nick))
		return;
	_members[userSocket] = nick;
	if (_members.size() == 1 && _operators.empty())
		_operators[userSocket] = nick;
}

void	Channel::addOperator(int userSocket, std::string &nick)
{
	if (_operators.find(userSocket) == _operators.end())
		_operators[userSocket] = nick;
}

void Channel::broadcast(int userSocket, std::string Msg)
{
	std::cout << Msg << std::endl;
	for (auto it = _members.begin(); it != _members.end(); it++)
		if (userSocket != it->first)
			sendTo(it->first, Msg);
}

void Channel::updateNickname(int socket, const std::string& newNick)
{
	if (_members.count(socket))
		_members[socket] = newNick;
	if (_operators.count(socket))
		_operators[socket] = newNick;
}

void Channel::removeOperator(int userSocket)
{
	_operators.erase(userSocket);
}

void Channel::removeMember(int userSocket)
{
	_members.erase(userSocket);
	_operators.erase(userSocket);
}
