/*
 * TCPServer.h
 *
 *  Created on: Aug 7, 2014
 *      Author: john
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

extern "C"
{
#include <netdb.h>
#include <sys/socket.h>
}

#include <vector>
#include <string>

#define DEFAULT_RECV_SIZE 1024

class TCPServer
{
	public:
		enum class ServerError{ Socket, Bind, Listen, SetRecvTimeout, Accept };
		enum class ClientDisconnectedError{ };
		enum class RecvTimeoutError{ };
		enum class AcceptTimeoutError{ };

		TCPServer();
		virtual ~TCPServer();
		void start( const int port );
		void stop();
		void setRecvTimeout( const long s );
		void waitForClientConnection();
		std::string waitForStringRead();
		std::string waitForStringRead( const size_t max_size );
		std::vector<char> waitForBytesRead();
		std::vector<char> waitForBytesRead( const size_t max_size );
		bool checkAlive();
		void closeClient();

	private:
		void setup_server( const int port );
		void set_recv_timeout();

		int sock_fd;
		int cli_sock_fd;
		struct sockaddr_in serv_addr;
		long recv_timeout_s;
};

#endif /* TCPSERVER_H_ */
