/*Socket.hpp
* Connor Black
* Represents an common interface for using various socket types. 
* Decoupled from WinSock to allow for other platform dependent 
* implementations of sockets.
*/

#pragma once

#ifdef _WIN32
#include "WinSock.hpp"
#endif

#include <memory>

class Socket{
public:
	static const int BUFFER_SIZE = 10'000;

private:
	unsigned short const PORT = 27015;

#ifdef _WIN32
	std::unique_ptr<WinSock> socket = nullptr;
#endif

public:

	Socket() {
#ifdef _WIN32
		socket = std::make_unique<WinSock>();
#else
		throw std::exception("No socketing is supported on this systems current architecture");
#endif
	}

	/// <summary>
	/// Listen for a connection 
	/// </summary>
	/// <param name="ip">IP for the connection to establish on</param>
	void listen_for_connection(const char* ip) {
		socket->listen_for_connection(PORT, ip);
	}

	/// <summary>
	/// Recieve data from the other established socket
	/// </summary>
	/// <returns>A string with the message data</returns>
	std::string recieve_data() {
		return socket->recieve_data();
	}

	/// <summary>
	/// Send data to the other connection
	/// </summary>
	/// <param name="sendbuf">String holding the message to send</param>
	void send_data(std::string sendbuf) {
		socket->send_data(sendbuf);
	}

	/// <summary>
	/// Connect to a host who is listening for a client
	/// </summary>
	/// <param name="ip">IP to connect to</param>
	void connect_to_host(const char* ip) {
		socket->connect_to_host(PORT, ip);
	}
};