#include "WinSock.hpp"
using namespace std;

void WinSock::listen_for_connection(unsigned short port, const char* ip) {
	// initialize WSA
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw exception("Server failed to start WSA");
	}

	// Create the TCP socket
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Create the server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	inet_pton(AF_INET, ip, &(serverAddress.sin_addr));

	//Bind the socket
	if (bind(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		throw exception("Failed to bind socket");

	//Listen for 1 client but you can hold to listen to more
	if (listen(hSocket, 1) == SOCKET_ERROR)
		throw exception("Error listening on socket");

	cout << "Binded successfully" << endl;

	hAccepted = SOCKET_ERROR;
	while (hAccepted == SOCKET_ERROR) {
		hAccepted = accept(hSocket, NULL, NULL); //Keep accepting until a connection is established
	}
}

void WinSock::connect_to_host(unsigned short port, const char* ip) {
	// initialize WSA
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		throw exception("Server failed to start WSA");

	// Create the TCP socket
	hAccepted = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Create the server address
	sockaddr_in serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);

	inet_pton(AF_INET, ip, &(serverAddress.sin_addr));

	//connect the socket
	if (connect(hAccepted, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		throw exception("Failed to connect to host");
}

string WinSock::recieve_data() {
	recv(hAccepted, recvbuf, BUFFER_SIZE, 0);
	return recvbuf;
}

void WinSock::send_data(string sendbuf) {
	send(hAccepted, sendbuf.c_str(), (int)strlen(sendbuf.c_str()) + 1, 0); //+1 is to send the terminating zero
}
