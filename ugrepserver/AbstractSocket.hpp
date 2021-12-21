/*AbstractSocket.hpp
* Connor Black
* Represents a pure virtual abstract socket which can listen for a connection
* or connect to a host machine's port as well as send and recieve data.
*/

#pragma once
#include <string>

class AbstractSocket {
public:
	void virtual listen_for_connection(unsigned short port, const char* ip) = 0;
	std::string virtual recieve_data() = 0;
	void virtual send_data(std::string sendbuf) = 0;
	void virtual connect_to_host(unsigned short port, const char* ip) = 0;
};