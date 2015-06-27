/**
 * This file contains the Socket and SocketException classes.
 * @author Brian Butler
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "horizon/net/encode/dataEncoder.h"
#include "horizon/net/sub/SubServerMessage.h"

using namespace horizon::net::encode;
using namespace horizon::net::sub;

namespace horizon {
	namespace net {
		namespace sockets {

/** Enum for the type of socket. */ 
enum TypeSocket {
	/** The socket will block on calls. */
	BlockingSocket,
	/** The socket will not block on calls. */
	NonBlockingSocket
};

/** Enum for the type of protocol the socket uses. */ 
enum SocketStream {
	/** The socket will use the TCP protocol. */
	TCP,
	/** The socket will use the IP protocol. */
	UDP
};

/**
 * SocketException class for errors arising within Socket methods.
 */
class SocketException : public std::exception {
private:
	// This member holds the error message for the exception
	std::string _what;
public:
	/**
	 * Constructor for the SocketException class.
	 * @param what_arg The string representing the error message.
	 */
	SocketException(const std::string& what_arg) : _what(what_arg) {}

	/**
	 * Constructor for the SocketException class with error code.
	 * @param what_arg The string representing the error message.
	 * @param errorCode The WinSock error code of the error.
	 */
	SocketException(const std::string& what_arg, const int& errorCode) {
		char buffer [33];
		_itoa(errorCode,buffer,10);
		_what = what_arg + " WinSock error: " + std::string(buffer);	
	}

	/** 
	 * Returns the error message of the exception.
	 * @return The error message.
	 */
	virtual const char* what() const throw() {
		static std::string err_string;
		err_string = "SocketException -> " + _what;
		return err_string.c_str();
	}

	/** Destructor for the SocketException class. */
	virtual ~SocketException() throw() {}
};

/**
 * Wrapper for WinSock sockets.
 */
class Socket {
public:
	/**
	 * A destructor for the Socket class.
	 * The socket is only closed when the last copy of the class is destructed.
	 * This function will decrement the reference count by 1.
	 */
	virtual ~Socket();
	
	/**
	 * A copy constructor for the Socket class.
	 * This function will increment the reference count by 1.
	 */
	Socket(const Socket&);
	
	/**
	 * A copy assignment operator for the Socket class.
	 * This function will increment the reference count by 1.
	 */
	Socket& operator=(Socket&);

	/**
	 * Receive bytes from the socket.
	 * This function simply creates a string to be returned
	 * and calls ReceiveBytes(std::string&), passing the string to return in.
	 * @return A string containg the data read from the socket.
	 */
	std::string ReceiveBytes();

	/**
	 * Receive bytes from the socket into the specified string.
	 * @param byteStream The string to read data from the socket into.
	 */
	void ReceiveBytes(std::string& byteStream);

	/**
	 * Closes the socket.
	 */
	void Close();

	/**
	 * Sends the bytes stored in a string over the socket.
	 * @param s The string containing the data to be sent.
	 */
	void SendBytes(const std::string& s);

	/**
	 * Returns the port of the other end of the socket.
	 * @return The port of the other end of the socket.
	 */
	u_short getClientPort();

	/**
	 * Returns the address of the other end of the socket.
	 * @return The address of the other end of the socket.
	 */
	std::string getClientAddress();

	/**
	 * Reads a number of bytes from the socket into the specified buffer.
	 * @param buf The buffer to read data from the socket into.
	 * @param len The number of bytes to read.
	 * @return True if the read completed successfully.
	 */
	bool ReadBytesIntoBuffer(char* buf, int len);

	/**
	 * Reads a header (for communication with HDS) from the socket.
	 * Return A DATA_HEADER representing the current data in the socket.
	 */
	DATA_HEADER ReadHeader();

	/**
	 * Reads an int message from the socket.
	 * @return The int message read from the socket.
	 */
	int ReceiveMessage_Int();

	/**
	 * Reads a Profile<int> message from the socket.
	 * @return The Profile<int> message read from the socket.
	 */
	Profile<int> ReceiveMessage_IntProfile();

	/**
	 * Reads a double message from the socket.
	 * @return The double message read from the socket.
	 */
	double ReceiveMessage_Double();

	/**
	 * Reads a Profile<double> message from the socket.
	 * @return The Profile<double> message read from the socket.
	 */
	Profile<double> ReceiveMessage_DoubleProfile();

	/**
	 * Reads a float message from the socket.
	 * @return The float message read from the socket.
	 */
	float ReceiveMessage_Float();

	/**
	 * Reads a Profile<float> message from the socket.
	 * @return The Profile<float> message read from the socket.
	 */
	Profile<float> ReceiveMessage_FloatProfile();

	/**
	 * Reads a bool message from the socket.
	 * @return The bool message read from the socket.
	 */
	bool ReceiveMessage_Bool();

	/**
	 * Reads a Profile<bool> message from the socket.
	 * @return The Profile<bool> message read from the socket.
	 */
	Profile<bool> ReceiveMessage_BoolProfile();

	/**
	 * Reads a Matrix message from the socket.
	 * @return The Matrix message read from the socket.
	 */
	Matrix ReceiveMessage_Matrix();

	/**
	 * Reads a Profile<Matrix> message from the socket.
	 * @return The Profile<Matrix> message read from the socket.
	 */
	Profile<Matrix> ReceiveMessage_MatrixProfile();

	/**
	 * Reads a Quat message from the socket.
	 * @return The Quat message read from the socket.
	 */
	Quat ReceiveMessage_Quat();

	/**
	 * Reads a Profile<Quat> message from the socket.
	 * @return The Profile<Quat> message read from the socket.
	 */
	Profile<Quat> ReceiveMessage_QuatProfile();

	/**
	 * Reads a header (for communication with HSS) from the socket.
	 * Return A DATA_HEADER_NS representing the current data in the socket.
	 */
	DATA_HEADER_NS ReadHeaderNS();

protected:
	friend class SocketServer;

	/**
	 * Constructor from a SOCKET handle.
	 * @param s The socket handle to create the Socket class with.
	 */
	Socket(SOCKET s);

	/**
	 * Constructor for the Socket class.
	 * @param stream The type of protocol the socket should use.
	 */
	Socket(SocketStream stream = TCP);

	/** Holder for the Socket handle. */
	SOCKET socket_;

	/** Pointer to an integer which holds the reference count for this Socket. */
	int* refCount;

private:
	/** The major version of WinSock library to use. */
	const static int MAJOR_VERSION = 2;
	/** The minor version of WinSock library to use. */
	const static int MINOR_VERSION = 2;

	/** Information about the client the socket is connected to. */
	static sockaddr_in* clientInfo_;

	/**
	 * Sets information about client socket.
	 * @param clientInfo The pointer to the structure of information about client connection.
	 */
	static void setClientInfo(sockaddr_in* clientInfo);
	
	/**
	 * Calls Winsock startup routing WSAStartup().
	 */
	static void Startup();

	/**
	 * Calls WinSock cleanup routing WSACleanup().
	 */
	static void Cleanup();

	/** Static variable to hold one more than the total number of Sockets created. */
	static int numSockets;
};

}}} // horizon::net::sockets

#endif