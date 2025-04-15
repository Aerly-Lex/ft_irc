#pragma once

#include "includes.hpp"

class User
{
	public:
		int				_socket;
		std::string		_nickname;
		std::string		_userName;
		std::string		_realName;
		std::string		_password;
		std::string		_hostName;
		std::string		_mask;
		std::string		_buffer;

		bool			_registered;
		std::string		_loginProcess;

		User() : _socket(-1) {}
		User(int socket) : _socket(socket) {}
		~User() {}
};
