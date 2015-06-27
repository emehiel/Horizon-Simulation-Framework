#include "Socket.h"
#include <iostream>
#include <sstream>

// Standard library usings
using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

namespace horizon {
	namespace net {
		namespace sockets {

// Max buffersize that can be received
#define BUF_SIZE 4096

// Initialize the number of Sockets created to 0
int Socket::numSockets = 0;

// Set client info pointer to null
sockaddr_in* Socket::clientInfo_ = NULL;

Socket::~Socket() {
	// Dereference and decrement the reference count
	// and check if it is equal to 0
	if (! --(*refCount)) {
		// Reference count is now zero, so close the Socket
		Close();
		// Delete the variable holding the reference count
		delete refCount;
	}
	// Decrement the number of Sockets 
	--numSockets;
	// If there are no more Sockets, call the Cleanup() routine
	if (!numSockets)
		Cleanup();
}

Socket::Socket(const Socket& sock) {
	// Copy the pointer to the reference count
	refCount = sock.refCount;
	// Increment the reference count
	(*refCount)++;
	// Copy the SOCKET handle
	socket_ = sock.socket_;
	// Increment the number of Sockets
	numSockets++;
}

Socket& Socket::operator=(Socket& sock) {
	// Increment the reference count
	(*sock.refCount)++;
	// Copy the pointer to the reference count
	refCount = sock.refCount;
	// Copy the SOCKET handle
	socket_ = sock.socket_;
	// Increment the number of Sockets
	numSockets++;
	// Return the Socket
	return *this;
}

string Socket::ReceiveBytes() {
	// Create a string to hold the received message
	string ret;
	// Call ReceiveBytes() passing the string in
	this->ReceiveBytes(ret);
	// Return the string
	return ret;
}

void Socket::ReceiveBytes(std::string& byteStream) {
	// Create a buffer to hold received data
	char buf[BUF_SIZE];
	// Variable to hold the number of bytes that have been read
	u_long total = 0;

	while(1) {
		// Varable that will hold the number of bytes that can be read atomically
		u_long arg = 0;
		// Call ioctlsocket(), throw an exception if the call fails
		if(ioctlsocket(socket_, FIONREAD, &arg) != 0) {
			int err = WSAGetLastError();
			throw SocketException("error in ReceiveBytes(): WinSock error: ", err);
		}

		// No more bytes to be read, return
		if(arg == 0 && total > 0)
			break;

		// No bytes can be read
		if(arg == 0) {
			// Sleep for a millisecond
			Sleep(1);
			// Check whether the connection is still alive by reading the first byte from the stream
			int alive = recv(socket_, buf, 1, 0);
			// Check if recv() resulted in an error
			if(alive == SOCKET_ERROR) {
				int err = WSAGetLastError();
				throw SocketException("error in ReceiveBytes(): WinSock error: ", err);
			}
			// Connection is alive, 
			if(alive > 0) {
				byteStream.push_back(buf[0]);
				++total; // BUG fix that caused to crash for 1 char messages
			}
			if(alive == 0) {
				break;
			}
			continue;
		}
		// More bytes are available to read then the buffer size
		// Set the amount to read to the buffer size
		if(arg > BUF_SIZE) arg = BUF_SIZE;

		// Call recv() reading arg bytes into the buffer
		int rv = recv(socket_, buf, arg, 0);

		// Check if recv() resulted in an error
		if(rv == SOCKET_ERROR) {
			int err = WSAGetLastError();
			throw SocketException("error in ReceiveBytes(): WinSock error: ", err);
		}

		// Add the number of bytes read in last recv() to the total number of bytes read
		total += rv;
		// Create a string and assign the contents of the buffer to the string
		string t;
		t.assign(buf, rv);
		// Add the contents to the string to byteStream
		byteStream += t;
	}
}

void Socket::Close() {
	// Close the Socket
	closesocket(socket_);
}

void Socket::SendBytes(const string& s) {
	// Variable to hold the number of bytes sent so far
	u_long totalSent = 0;
	// Check that the length of the message is non-zero
	if(s.length()) {
		// Continue until the total length sent equals the length of the string
		while(totalSent != s.length()) {
			int sent = 0;
			// Get the number of bytes sent in the call to send
			// The starting position to send from will be the start of the string + offset of how many bytes send so far
			// The number of bytes to send is the length of the string - how many bytes sent so far
			sent = send(socket_, (s.c_str())+totalSent, (int)s.length()-totalSent, 0);
			// If 0 bytes are sent, throw an error
			if(sent == 0) {
				throw SocketException("error in SendBytes()");
			}
			// Check if an error occured in the send
			if(sent == SOCKET_ERROR) {
				int err = WSAGetLastError();
				throw SocketException("error in SendBytes(): WinSock error: ", err);
			}
			totalSent += sent;
		}
	}
}

u_short Socket::getClientPort() {
	if(Socket::clientInfo_)
		return ntohs(Socket::clientInfo_->sin_port);
	else
		return 0;
}

std::string Socket::getClientAddress() {
	if(Socket::clientInfo_)
		return std::string(inet_ntoa(Socket::clientInfo_->sin_addr));
	else
		return "";
}

Socket::Socket(SOCKET s) : socket_(s) {
	// Call the Startup() routine
	Startup();
	// Initialize the reference counter
	refCount = new int(1);
};

Socket::Socket(SocketStream stream) : socket_(0) {
	// Call the Startup() routine
	Startup();
	// Create the socket
	// Check if the type of socket is TCP or UDP
	// If TCP, then the type parameter needs to be SOCK_STREAM
	// If UDP, then the type parameter needs to be SOCK_DGRAM
	if(stream == TCP)
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
	else if(stream == UDP)
		socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	else {
		ostringstream oss;
		oss << "unknown socket stream type [" << stream << "]";
		throw SocketException(oss.str());
	}
	// Check if the SOCKET is valid
	if(socket_ == INVALID_SOCKET) {
		throw "INVALID_SOCKET";
	}
	// Initialize the reference counter
	refCount = new int(1);
}

void Socket::setClientInfo(sockaddr_in* clientInfo) {
	Socket::clientInfo_ = clientInfo;
}

void Socket::Startup() {
	if (!numSockets) {
		// Declare the structure which will contain information
		// about the WinSock DLL after the call to WSAStartup()
		WSADATA wsaData;
		// Call WSAStartup() and get the returned error code
		// An error occurs if the return value does not equal 0
		int err = WSAStartup(MAKEWORD(MAJOR_VERSION, MINOR_VERSION), &wsaData);
		// Check for any errors
		if(err != 0)
			throw SocketException("Could not start WinSock");
		// Check that the version stored in the wVersion field of
		// the WSADATA structure is what we requested
		if(LOBYTE(wsaData.wVersion) != MAJOR_VERSION || HIBYTE(wsaData.wVersion) != MINOR_VERSION) {
			// No usable WinSock DLL found, call WSACleanup() and throw exception
			WSACleanup();
			throw SocketException("No useable WinSock DLL found!");
		}
		// WinSock startup successful!
	}
	// Increment the number of Sockets created.
	++numSockets;
}

void Socket::Cleanup() {
	// Delete clientInfo_ if necessary
	if(Socket::clientInfo_)
		delete Socket::clientInfo_;
	// Call WSACleanup()
	WSACleanup();
}

bool Socket::ReadBytesIntoBuffer(char* buf, int len) {
	// Variable to hold the number of bytes that have been read
	u_long bytesRead = 0;
	u_long bytesToRead = len;
	while(1) {
		// Varable that will hold the number of bytes that can be read atomically
		u_long arg = 0;
		// Call ioctlsocket(), throw an exception if the call fails
		if(ioctlsocket(socket_, FIONREAD, &arg) != 0) {
			int err = WSAGetLastError();
			throw SocketException("error in ReceiveMessage(): WinSock error: ", err);
		}

		if(bytesRead == bytesToRead)
			break;

		// No bytes can be read
		if(arg == 0) {
			// Sleep for a millisecond
			Sleep(1);
			// Check whether the connection is still alive by reading the first byte from the stream
			int alive = recv(socket_, buf+bytesRead, 1, 0);
			// Check if recv() resulted in an error
			if(alive == SOCKET_ERROR) {
				int err = WSAGetLastError();
				if(err == WSAEWOULDBLOCK) {
					continue;
				}
				throw SocketException("error in ReceiveMessage(): WinSock error: ", err);
			}
			if(alive == 0) {
				break;
			}
			bytesRead++;
			continue;
		}
		// More bytes are available to read then the buffer size
		// Set the amount to read to the buffer size
		if(arg > bytesToRead - bytesRead)
			arg = bytesToRead - bytesRead;

		// Call recv() reading arg bytes into the buffer
		int rv = recv(socket_, buf+bytesRead, arg, 0);

		// Check if recv() resulted in an error
		if(rv == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if(err == WSAEWOULDBLOCK) {
					continue;
			}
			throw SocketException("error in ReceiveBytes(): WinSock error: ", err);
		}

		// Add the number of bytes read in last recv() to the total number of bytes read
		bytesRead += rv;
	}
}

DATA_HEADER Socket::ReadHeader() {
	// Variable to hold the number of bytes that have been read
	u_long bytesRead = 0;
	u_long bytesToRead = TYPE_BYTES + SIZE_BYTES;
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	DATA_HEADER header;
	memcpy(&header.type, buf, TYPE_BYTES);
	memcpy(&header.size, buf+TYPE_BYTES, SIZE_BYTES);
	delete[] buf;
	return header;
}

int Socket::ReceiveMessage_Int() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	int ret = decodeIntMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<int> Socket::ReceiveMessage_IntProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<int> ret = decodeIntProfileMessageData(buf);
	delete[] buf;
	return ret;
}

double Socket::ReceiveMessage_Double() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	double ret = decodeDoubleMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<double> Socket::ReceiveMessage_DoubleProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<double> ret = decodeDoubleProfileMessageData(buf);
	delete[] buf;
	return ret;
}

float Socket::ReceiveMessage_Float() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	float ret = decodeFloatMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<float> Socket::ReceiveMessage_FloatProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<float> ret = decodeFloatProfileMessageData(buf);
	delete[] buf;
	return ret;
}

bool Socket::ReceiveMessage_Bool() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	bool ret = decodeBoolMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<bool> Socket::ReceiveMessage_BoolProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<bool> ret = decodeBoolProfileMessageData(buf);
	delete[] buf;
	return ret;
}

Matrix Socket::ReceiveMessage_Matrix() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Matrix ret = decodeMatrixMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<Matrix> Socket::ReceiveMessage_MatrixProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<Matrix> ret = decodeMatrixProfileMessageData(buf);
	delete[] buf;
	return ret;
}

Quat Socket::ReceiveMessage_Quat() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Quat ret = decodeQuatMessageData(buf);
	delete[] buf;
	return ret;
}

Profile<Quat> Socket::ReceiveMessage_QuatProfile() {
	// Read in the header
	DATA_HEADER header = ReadHeader();
	// Now calculate the required number of bytes left to read
	u_long bytesRead = 0;
	u_long bytesToRead = header.size - (TYPE_BYTES + SIZE_BYTES);
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	Profile<Quat> ret = decodeQuatProfileMessageData(buf);
	delete[] buf;
	return ret;
}

DATA_HEADER_NS Socket::ReadHeaderNS() {
	// Variable to hold the number of bytes that have been read
	u_long bytesRead = 0;
	u_long bytesToRead = 2*TYPE_BYTES + SIZE_BYTES;
	// Create a buffer to hold received data
	char* buf = new char[bytesToRead];
	// Read rest of message into buffer
	ReadBytesIntoBuffer(buf, bytesToRead);
	DATA_HEADER_NS header;
	memcpy(&header.type, buf, TYPE_BYTES);
	memcpy(&header.stubType, buf + TYPE_BYTES, TYPE_BYTES);
	memcpy(&header.size, buf + 2*TYPE_BYTES, SIZE_BYTES);
	delete[] buf;
	return header;
}

}}} // horizon::net::sockets