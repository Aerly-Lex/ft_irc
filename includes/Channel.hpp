#pragma once

#include "includes.hpp"

class User;

class Channel
{
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_topicData;
		std::string					_password;
		int							_userLimit;

		bool						_inviteOnly;
		bool						_topic_rigths;

		std::vector<std::string>	_banned;
		std::set<std::string>		_invited;
		std::map<int, std::string>	_members;
		std::map<int, std::string>	_operators;

	public:
		Channel();
		Channel(const std::string& name);
		~Channel();

		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getTopicData() const;
		std::string			getPass() const;
		int					getUserLimit() const;
		bool				isInviteOnly() const;
		bool				isTopicRights() const;

		std::string			getNames() const;

		bool				isInvited(std::string &user) const;


		void				setName(std::string &name);
		void				setTopic(std::string &topic);
		void				setPass(std::string &password);
		void				setUserLimits(int &userLimit);
		void				setInviteOnly(bool mode);
		void				setTopicRights(bool rights);

		void				broadcast(int userSocket, std::string Msg);
		bool				isMember(int socket) const;
		void				updateNickname(int socket, const std::string &newNick);

		void				banUser(std::string &nick);
		bool				isBanned(const std::string &nick) const;
		bool				memberExists(const std::string &nick) const;

		void				addMember(int userSocket, const std::string &nick);
		bool				isMember(int socket) const;
		int					isMember(std::string nickName);
		void				removeMember(int userSocket);

		void				addOperator(int userSocket, std::string &nick);
		bool				isOperator(int userSocket) const;
		void				removeOperator(int userSocketk);
		// void	removeMember(int clientSocket);
		// void	broadcastMessage(int clientSocket, const std::string& message);
};
