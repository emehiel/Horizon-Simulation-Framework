#include "SocketClient.h"

namespace horizon {
	namespace net {
		namespace sockets {

SocketClient::SocketClient() : Socket() {}

SocketClient::SocketClient(const std::string& host, int port) : Socket() {
	std::string error;

	hostent *he;
	if ((he = gethostbyname(host.c_str())) == 0) {
		error = strerror(errno);
		throw SocketException("Error connecting to server: " + error, errno);
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((in_addr *)he->h_addr);
	memset(&(addr.sin_zero), 0, 8); 

	if (::connect(socket_, (sockaddr *) &addr, sizeof(sockaddr))) {
		error = strerror(WSAGetLastError());
		throw SocketException("Error connecting to server: " + error, errno);
	}
}
		
}}} // horizon::net::sockets