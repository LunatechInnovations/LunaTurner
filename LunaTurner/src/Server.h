/*
 * Server.h
 *
 *  Created on: Apr 3, 2014
 *      Author: john
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>

extern "C"
{
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
}

class Server
{
	public:
		Server( const int port );
		virtual ~Server();
		bool waitForConnection();
		const std::string waitForAngles();

	private:
		struct addrinfo *host_info_list;
		int sock_fd;
		int active_sock_fd;
		bool has_connection;
		bool checkAlive();
};

#endif /* SERVER_H_ */
