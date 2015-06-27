/**
 * This file contains the SocketClient class.
 * @author Brian Butler
 */

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "Socket.h"

namespace horizon {
	namespace net {
		namespace sockets {

/**
 * This class acts as the client side socket for communication with the data server.
 * @author Brian Butler
 */
class SocketClient : public Socket {
public:
	/** Default constructor for the SocketClient class. */
	SocketClient();

	/**
	 * Constructor for the SocketClient class.
	 * @param host The host to connect the socket to.
	 * @param port The port on the server to connect to.
	 */
	SocketClient(const std::string& host, int port);
};

}}} // horizon::net::sockets

#endif