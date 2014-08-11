/*
 * TCPServer.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: john
 */

#include "TCPServer.h"
#include <cstring>
#include <sstream>
#include <iterator>

#include <iostream>

extern "C"
{
#include <unistd.h>
#include <errno.h>
}

/* TCPServer
 * Constructor
 */
TCPServer::TCPServer() : sock_fd{ -1 }, cli_sock_fd{ -1 }, recv_timeout_s{ -1 }
{
}

/* ~TCPServer
 * Destructor
 */
TCPServer::~TCPServer()
{
	stop();
}

/* start
 * Setup server address, create socket, set socket options,
 * bind address to socket and start listening on the specified port
 */
void TCPServer::start( const int port )
{
	setup_server( port );

	if( recv_timeout_s > 0 )
		set_recv_timeout();

	if( (bind( sock_fd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) )) < 0 )
		throw ServerError::Bind;

	if( (listen( sock_fd, 5 )) < 0 )
		throw ServerError::Listen;
}

/* setRecvTimeout
 * Setter for timeout value in seconds for read call
 */
void TCPServer::setRecvTimeout( const long s )
{
	recv_timeout_s = s;
}

/* waitForClientConnection
 * Calls accept if client is not already connected
 */
void TCPServer::waitForClientConnection()
{
	if( cli_sock_fd >= 0 )
		return;

	cli_sock_fd = accept( sock_fd, nullptr, 0 );

	if( cli_sock_fd < 0 )
	{
		if( errno == ETIMEDOUT || errno == EAGAIN )
			throw AcceptTimeoutError();
		else
			throw ServerError::Accept;
	}
}

/* setup_server
 * Create server socket and setup server address
 */
void TCPServer::setup_server( const int port )
{
	sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock_fd < 0 )
		throw ServerError::Socket;

	//Setup server address structure
	memset( &serv_addr, 0, sizeof( struct sockaddr_in ) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( port );
}

/* waitForStringRead
 * Helper function to convert bytes read to a string
 */
std::string TCPServer::waitForStringRead()
{
	return waitForStringRead( DEFAULT_RECV_SIZE );
}

std::string TCPServer::waitForStringRead( const size_t max_size )
{
	std::vector<char> in = waitForBytesRead( max_size );
	for( auto i : in )
		std::cout << i << std::endl;

	std::ostringstream ret;

	if( !in.empty() )
		std::copy( in.begin(), in.end(), std::ostream_iterator<char>( ret ) );

	return ret.str();
}

std::vector<char> TCPServer::waitForBytesRead()
{
	return waitForBytesRead( DEFAULT_RECV_SIZE );
}

std::vector<char> TCPServer::waitForBytesRead( const size_t max_size )
{
	char buffer[max_size];
	int recv_sz = read( cli_sock_fd, buffer, max_size );

	if( recv_sz > 0 )
	{
		std::vector<char> ret{ buffer, buffer + recv_sz };
		return ret;
	}
	else if( recv_sz <= 0 )
	{
		if( !checkAlive() )
		{
			closeClient();
			throw ClientDisconnectedError();
		}
		else
		{
			if( recv_sz < 0 )
				throw RecvTimeoutError();
		}
	}

	return std::vector<char>();
}

/* checkAlive
 * Send enquiry character to determine if client is still connected
 */
bool TCPServer::checkAlive()
{
	if( cli_sock_fd < 0 )
		return false;

	char dummy{ '\5' };
	return (send( cli_sock_fd, (void *)&dummy, sizeof( char ), 0) > 0);
}

/* closeClient
 * Close client socket if open
 */
void TCPServer::closeClient()
{
	if( cli_sock_fd >= 0 )
	{
		close( cli_sock_fd );
		cli_sock_fd = -1;
	}
}

/* stop
 * Stops server by closing client and server sockets
 */
void TCPServer::stop()
{
	closeClient();
	close( sock_fd );
	sock_fd = -1;
}

void TCPServer::set_recv_timeout()
{
	struct timeval to;
	to.tv_sec = recv_timeout_s;
	to.tv_usec = 0;

	if( (setsockopt( sock_fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&to, sizeof( struct timeval ) )) < 0 )
		throw ServerError::SetRecvTimeout;
}
