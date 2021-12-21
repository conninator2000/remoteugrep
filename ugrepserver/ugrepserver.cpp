/*ugrepserver.cpp
* Connor Black
* A remote ultragrep server that communicates with a client using TCP/IP and recieves
* a command from the client which then executes on the server side.
*/


#include<sstream>
#include "Socket.hpp"
using namespace std;

#include <iostream>

int main(int argc, char* argv[]) try {
	cout << "Ultragrep Server - Connor Black 2021\n";

	//Check if there was a IP given by parameter
	string ip = "127.0.0.1";

	if (argc > 1)
		ip = argv[1];

	Socket sock = Socket();

	cout << "Listening for client connection on ip " << ip << "..." << endl<<endl;
	sock.listen_for_connection(ip.c_str());

	cout << "TCP/IP Socket bound" << endl;

	string result;
	while (true) {
		//Recieve initial command
		cout << "Waiting for command..." << endl;
		result = "";
		result = sock.recieve_data();
		
		//Client filters this kind of input out. This will hit if the client is closed before any data is passed
		if (result == "") {
			cout << "Error with client connection. Exiting now." << endl;
			break;
		}
		cout << "Recieved command: " << result << endl<<endl;

		if (result == ":quit")
			break;

		//Send status message
		sock.send_data("Executing "+ result);
		cout << "Sent program execution status"<<endl;
		cout << "Executing command '" << result << "'" << endl<<endl;

		ostringstream oss;

		//Execute with a pipe
		FILE* pPipe = _popen(result.c_str(), "rt"); //creates a pipe between shell and program - runs dir in r - read t - text mode
		if (pPipe == NULL)
			throw exception("Could not open pipe to shell");

		int ch;
		while ((ch = fgetc(pPipe)) != EOF)
			oss.put(ch);
		_pclose(pPipe);
		string s = oss.str();
		cout << "Captured " << s.size() << " bytes" << endl << endl;;

		if (s.size() == 0) 
			s = "Error executing command " + result;

		cout << s << endl;
		/*for (auto c : s)
			cout.put(c);*/

		//Determine if the output can be sent with the current values 
		if (s.size() + 1 < Socket::BUFFER_SIZE) {
			sock.send_data(s);
			cout << "Sent single block of data to client" << endl;

			sock.send_data("0");
			cout << "Sent final data transfer confirmation" << endl<<endl;
		}
		else { //Split up the data for the message and continuously send it within the range of the current server buffer size
			unsigned int indx = 0;
			const unsigned int range = Socket::BUFFER_SIZE; //save the last byte for the terminating zero on each send
			unsigned int elementsToSend = s.size() / 10;

			if (s.size() % 10) //Check if there is a remainder
				++elementsToSend;

			do {
				//Get a substring that is within the sending range
				string substr = s.substr(indx, range);

				unsigned int element = (indx / range) + 1;
				if (element > elementsToSend)
					element = elementsToSend;

				sock.send_data(substr);
				cout << "Sending data block [" << element << "/" << elementsToSend << "]" << endl;

				//Is there more blocks to send
				if (indx + range < s.size()) {
					sock.send_data("1");
					indx += range;
				}
				else {
					sock.send_data("0");
					indx = s.size();
					cout << "Sent final data transfer confirmation" << endl<<endl;
				}
			} while (indx != s.size());
		}
	}

	cout << "Shutting down server" << endl;
}
catch (exception e) {
	cerr << e.what() << endl;
	return EXIT_FAILURE;
}