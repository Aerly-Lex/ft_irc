#pragma once

#include "includes.hpp"

class Client
{
	public:
		int				_socket;
		std::string		_nickName;
		std::string		_userName;
		std::string		_realName;
		std::string		_password;
		std::string		_hostName;
		std::string		_ipaddress;
		std::string		_recvMesg;

		bool			_registered;

		Client() : _socket(-1) {}
		Client(int socket) : _socket(socket) {}
		~Client() {}
};
