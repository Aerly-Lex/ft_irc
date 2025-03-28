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
		// int					userLimit;

		std::vector<int>	members;
		//operators?

		Channel();
		Channel(const std::string& name);
		~Channel();

		// void	addMember(int clientSocket);
		// void	removeMember(int clientSocket);
		// void	broadcastMessage(int clientSocket, const std::string& message);
};
