#include "SubServerMessage.h"

namespace horizon {
	namespace net {
		namespace sub {

char* encodeDataHeaderNSEx(DATA_HEADER_NS& header, uint32_t extra, uint32_t& msgSize) {
	// Calculate the size of the message
	msgSize = 2*TYPE_BYTES + SIZE_BYTES + extra;
	// Set the size
	header.size = msgSize;
	// Initialize buffer
	char* buf = new char[msgSize];
	// Copy type
	memcpy(buf, &header.type, TYPE_BYTES);
	// Copy stub type
	memcpy(buf + TYPE_BYTES, &header.stubType, TYPE_BYTES);
	// Copy size
	memcpy(buf + 2*TYPE_BYTES, &header.size, SIZE_BYTES);
	// Return buffer, must be deleted by who it is returned to
	return buf;
}

DATA_HEADER_NS decodeDataHeaderNS(char* buf) {
	DATA_HEADER_NS header;
	// Copy type
	memcpy(&header.type, buf, TYPE_BYTES);
	// Copy stub type
	memcpy(&header.stubType, buf + TYPE_BYTES, TYPE_BYTES);
	// Copy size
	memcpy(&header.size, buf + 2*TYPE_BYTES, SIZE_BYTES);
	// Return header
	return header;
}

}}} // horizon::net::sub