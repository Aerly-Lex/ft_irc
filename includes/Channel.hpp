#pragma once

#include "includes.hpp"

class Channel
{
	public:
		std::string			_name;
		std::string			_topic;
		std::string			_password;
		std::string			_mode;
		bool				passwordProtected;
		bool				inviteOnly;

		std::vector<int>	_members;

		Channel(const std::string& name);

		void	addMember(int clientSocket);
		void	removeMember(int clientSocket);
		// void	broadcastMessage(int clientSocket, const std::string& message);
};
