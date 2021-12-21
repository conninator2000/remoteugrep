/*ugrepclient.cpp
* Connor Black
* A remote ultragrep client that communicates with a server using TCP/IP and sends
* a command to the server which then executes the command and returns the data in blocks if its over a specified amount
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include "Socket.hpp"

using namespace std;

#pragma comment (lib,"ws2_32.lib")

unsigned short constexpr PORT = 27015;

int main(int argc, char* argv[]) try {
	cout << "Ultragrep Client - Connor Black 2021\n";

	Socket sock;

	//Check if there was a IP listed
	string ip = "127.0.0.1";

	if (argc > 1)
		ip = argv[1];

	sock.connect_to_host(ip.c_str());
	cout << "Connected to the ultragrep server." << endl;

	string command, result;
	while (true) {
		cout << endl;
		cout << "Enter the command to execute or ':quit' to quit :";
		getline(cin, command);
		if (command == "")
			continue;

		cout << endl;
		//Send command to server
		sock.send_data(command);

		if (command == ":quit")
			break;

		//Server will only respond to the quit if it did not quit
		//Recieve launch status from server
		result = sock.recieve_data();

		//If the server returns 0 then that most likely means that the socket is no longer connected to the host
		// "" - hit if the server is closed before any commands are passed but after a connection is established
		// 0 - the server has sent data prior but has since closed its connection
		if (result == "0" || result == "") {
			cout << "The server might have disconnected" << endl;
			break;
		}


		cout << "Recieved command: " << result << endl<<endl;
		//strcpy_s(recvmsg, sock.recieve_data());
		//cout << recvmsg << endl;
		cout << "Response from server:" << endl;
		string moreData;
		//Loop retrieving data while the server passes a value to indicate more data to be read
		do {
			//Recieve the data from the server
			result = sock.recieve_data();
			cout << result;

			//Recieve the status on the server message (if more is coming)
			moreData = sock.recieve_data();
		} while (result == "1");
	}

	cout << "Closing connection" << endl;
}
catch (exception e) {
	cerr << e.what()<<endl;
	return EXIT_FAILURE;
}