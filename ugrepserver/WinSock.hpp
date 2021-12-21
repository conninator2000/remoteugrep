/*WinSock.hpp
* Connor Black
* A class representing a WinSock2 socket that uses the abstract socket pure virtual class to give it a common interface
*/

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <exception>
#include "AbstractSocket.hpp"
#include <iostream>
#pragma comment (lib,"ws2_32.lib")

class WinSock : public AbstractSocket {
private:
	static const int BUFFER_SIZE = 10'000;
	WSAData wsaData;
	SOCKET hSocket = SOCKET_ERROR; //Host socket
	sockaddr_in serverAddress = { 0 };
	SOCKET hAccepted = SOCKET_ERROR; //Client connection
	char sendbuf[BUFFER_SIZE] = "";
	char recvbuf[BUFFER_SIZE] = "";

public:

	~WinSock() {
		WSACleanup();

		if (hSocket != SOCKET_ERROR)
			closesocket(hSocket);

		if (hAccepted != SOCKET_ERROR)
			closesocket(hAccepted);
	}

	/// <summary>
	/// Listen for an incoming connection from a client
	/// </summary>
	/// <param name="port">The port to listen to</param>
	/// <param name="ip">The ip of the host machine</param>
	void listen_for_connection(unsigned short port, const char* ip);

	/// <summary>
	/// Connect to a host who is listening for a connection
	/// </summary>
	/// <param name="port">Port to connect to</param>
	/// <param name="ip">the ip of the host machine</param>
	void connect_to_host(unsigned short port, const char* ip);

	/// <summary>
	/// String data to receive from the other machine
	/// </summary>
	/// <returns>A string with the response data</returns>
	std::string recieve_data();

	/// <summary>
	/// Send a string message to a machine awaiting a message
	/// </summary>
	/// <param name="sendbuf">String containing the message</param>
	void send_data(std::string sendbuf);
};