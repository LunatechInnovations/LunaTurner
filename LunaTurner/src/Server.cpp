/*
 * Server.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: john
 */

#include "Server.h"
#include <cstring>
#include <iostream>

Server::Server( const int port ) : active_sock_fd{ -1 }, has_connection{ false }
{
	struct addrinfo host_info;
	memset( &host_info, 0, sizeof( struct addrinfo ) );

	host_info.ai_flags = AI_PASSIVE;


	//Retrieve interfaces
	int status = getaddrinfo( nullptr, std::to_string( port ).c_str(), &host_info, &host_info_list );
	if( status != 0 )
	{
		throw std::string{ "getaddrinfo failed" };
	}

	//Create socket
	sock_fd = socket( host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol );
	if( sock_fd == -1 )
	{
		throw std::string{ "socket failed" };
	}

	//TCP socket
	int yes = 1;
	status = setsockopt( sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) );
	if( status == -1 )
	{
		throw std::string{ "setsockopt failed (REUSEADDR)" };
	}

	//Set timeout of recv
	struct timeval recv_timeout;
	recv_timeout.tv_sec = 10;
	recv_timeout.tv_usec = 0;
	status = setsockopt( sock_fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&recv_timeout, sizeof( struct timeval ) );
	if( status == -1 )
	{
		throw std::string{ "setsockopt failed (RCVTIMEO)" };
	}

	//Bind to the first interface
	status = bind( sock_fd, host_info_list->ai_addr, host_info_list->ai_addrlen );
	if( status == -1 )
	{
		throw std::string{ "bind failed" };
	}

	//Start listening
	status = listen( sock_fd, 1 );
	if( status == -1 )
	{
		throw std::string{ "listen failed" };
	}
}

Server::~Server()
{
}

bool Server::waitForConnection()
{
	if( has_connection )
		return true;
	else
		active_sock_fd = -1;

	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof( client_addr );

	active_sock_fd = accept( sock_fd, (struct sockaddr *)&client_addr, &addr_size );
	if( active_sock_fd == -1 )
	{
		return false;
		has_connection = false;
	}

	has_connection = true;

	return true;
}

const std::string Server::waitForAngles()
{
	ssize_t bytes_received;
	char rx_buff[1024];

	bytes_received = recv( active_sock_fd, &rx_buff, sizeof( rx_buff ), 0 );
	if( bytes_received == 0 )
	{
		return std::string();
	}
	else if( bytes_received <= -1 )
	{
		if( !checkAlive() )
			has_connection = false;

		return std::string();
	}

	return std::string( rx_buff );
}

bool Server::checkAlive()
{
	if( !has_connection )
		return false;

	char dummy{ '*' };
	return send( active_sock_fd, (void *)&dummy, sizeof( char ), 0 ) != -1;
}
