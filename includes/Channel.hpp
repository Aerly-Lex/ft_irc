#pragma once

#include "includes.hpp"

class User;

class Channel
{
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_password;
		std::string					_mode;
		std::vector<std::string>	_banned;
		std::map<int, std::string>	_members;
		std::map<int, std::string>	_operators;

	public:
		bool				inviteOnly;
		// int					userLimit;
		Channel();
		Channel(const std::string& name);
		~Channel();

		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getPass() const;
		std::string			getMode() const;
		std::string			getNames() const;
		
		void				broadcast(int userSocket, std::string Msg);

		void				setName(std::string &name);
		void				setTopic(std::string &topic);
		void				setPass(std::string &password);
		void				setMode(std::string &mode);

		void				banUser(std::string &nick);
		bool				isBanned(const std::string &nick) const;

		void				addMember(int userSocket, std::string &nick);
		void				removeMember(int userSocket);

		void				addOperator(int userSocket, std::string &nick);
		void				removeOperator(int userSocketk);
		// void	removeMember(int clientSocket);
		// void	broadcastMessage(int clientSocket, const std::string& message);
};
