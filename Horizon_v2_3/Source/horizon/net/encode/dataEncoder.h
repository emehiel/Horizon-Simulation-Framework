/**
 * This file contains helper methods for communicating messages.
 * @author Brian Butler
 */

#ifndef DATAENCODER_H
#define DATAENCODER_H

// HSF Includes
#include "horizon/stdint.h"
#include "horizon/util/util.h"
#include "horizon/util/Quat.h"
#include "horizon/util/Matrix.h"
#include "horizon/util/Profile.h"

// C++ Includes
#include <string>
#include <iostream>
#include <crtdbg.h>
#include <deque>
#include <map>

// Windows Include
#include <windows.h>

// HSF Usings
using namespace horizon::util;

// C++ Usings
using namespace std;

namespace horizon {
	namespace net {
		namespace encode {

/** Number of bytes in type field of DATA_HEADER */
#define TYPE_BYTES sizeof(BYTE)
/** Number of bytes in size field of DATA_HEADER */
#define SIZE_BYTES sizeof(uint32_t)

/**
 * Enumeration of data type bit flags for type field of DATA_HEADER
 */
enum DATA_TYPE {
	/** error bit flag		0x00 */
	_ERROR = 0,
	/** int bit flag		0x01 */
	_INT = 1,
	/** double bit flag	0x02 */
	_DOUBLE = 2,
	/** float bit flag		0x04 */
	_FLOAT = 4,
	/** bool bit flag		0x08 */
	_BOOL = 8,
	/** matrix	bit flag	0x10 */
	_MATRIX = 16,
	/** quat bit flag		0x20 */
	_QUAT = 32
};

/**
 * Enumeration of profile bit flag for type field of DATA_HEADER
 */
enum PROF_FLAG {
	/** not profile flag	0x00 */
	_NOT_PROF = 0,
	/** profile flag		0x80 */
	_PROF = 128
};

/**
 * Enumeration of message bit flag for type field of DATA_HEADER
 */
enum MSG_FLAG {
	/** not message flag	0x00 */
	_NOT_MSG = 0,
	/** message flag		0x40 */
	_MSG = 64
};

/**
 * Enumeration of connection test bit flag for type field of DATA_HEADER
 */
enum CONN_TEST_FLAG {
	/** not test flag		0x00 */
	_NO_TEST = 0,
	/** test flag			0x3F */
	_YES_TEST = 63
};

/**
 * A data header for all data sent over network.
 */
struct DATA_HEADER {
	/** A byte to hold flags about data. */
	BYTE type;
	/** The total size of data in bytes(including header). */
	uint32_t size;
};

/**
 * A message data structure sent over network.
 */
struct MSG_DATA {
	/** The header for the message. */
	DATA_HEADER header;
	/** The type of DataHandler that should be used to process this message. */
	uint32_t handlerType;
	/** Start time for data to retrieve. */
	double start;
	/** End time for data to retrieve(only used for Profile return types). */
	double end;
	/** The variable to be retrieved. */
	string var;
};

/**
 * Returns the DATA_TYPE of the header.
 * @return The DATA_TYPE of the header.
 */
DATA_TYPE getDataType(const DATA_HEADER& header);

/**
 * Convertes between DATA_TYPE and varType enumerations.
 * @param type The DATA_TYPE representation of the data type.
 * @return The varType representation of the data type.
 */
varType DATA_TYPE_to_varType(DATA_TYPE type);

/**
 * Checks if the data header has a data type flag set.
 * @param header The DATA_HEADER to check the type of.
 * @param type The DATA_TYPE flag to check.
 * @return True if the header has the type flag set, false otherwise.
 */
bool isDataType(const DATA_HEADER& header, const DATA_TYPE& type);

/**
 * Checks if the data header has a Profile flag set.
 * @param header The DATA_HEADER to check.
 * @return True if the header has the Profile flag (_PROF) set, false otherwise.
 */
bool isProfile(const DATA_HEADER& header);

/**
 * Checks if the data header has is a connection test.
 * A connection test header has the _MSG and _YES_TEST flags set.
 * @param header The DATA_HEADER to check.
 * @return True if the header is a connection test, false otherwise.
 */
bool isConnectionTest(const DATA_HEADER& header);

/**
 * Checks if the data header is a message.
 * @param header The DATA_HEADER to check.
 * @return True if the header has the message flag (_MSG) set, false otherwise.
 */
bool isMessage(const DATA_HEADER& header);

/**
 * Returns a connection test message to be sent.
 * @return A connection test message to be sent.
 */
string createConnectionTestMessage();

/**
 * Encodes the message structure into a string to be sent over network.
 * @param msg The message ot be encoded.
 * @return A string holding the message data.
 */
string encodeMessage(MSG_DATA& msg);

/**
 * Reads an int from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The int value read from the buffer.
 */
int decodeIntMessageData(const char* buf);

/**
 * Reads a Profile<int> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<int> value read from the buffer.
 */
Profile<int> decodeIntProfileMessageData(const char* buf);

/**
 * Reads a double from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The double value read from the buffer.
 */
double decodeDoubleMessageData(const char* buf);

/**
 * Reads a Profile<double> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<double> value read from the buffer.
 */
Profile<double> decodeDoubleProfileMessageData(const char* buf);

/**
 * Reads a float from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The float value read from the buffer.
 */
float decodeFloatMessageData(const char* buf);

/**
 * Reads a Profile<float> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<float> value read from the buffer.
 */
Profile<float> decodeFloatProfileMessageData(const char* buf);

/**
 * Reads a bool from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The bool value read from the buffer.
 */
bool decodeBoolMessageData(const char* buf);

/**
 * Reads a Profile<bool> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<bool> value read from the buffer.
 */
Profile<bool> decodeBoolProfileMessageData(const char* buf);

/**
 * Reads a Matrix from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Matrix value read from the buffer.
 */
Matrix decodeMatrixMessageData(const char* buf);

/**
 * Reads a Profile<Matrix> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<Matrix> value read from the buffer.
 */
Profile<Matrix> decodeMatrixProfileMessageData(const char* buf);

/**
 * Reads a Quat from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Quat value read from the buffer.
 */
Quat decodeQuatMessageData(const char* buf);

/**
 * Reads a Profile<Quat> from the specified buffer.
 * @parm buf The buffer to read from.
 * @return The Profile<Quat> value read from the buffer.
 */
Profile<Quat> decodeQuatProfileMessageData(const char* buf);

uint32_t encodeIntMessage(char* buf, const uint32_t& val);

uint32_t encodeIntProfileMessage(char* buf, Profile<int>& val);

uint32_t encodeDoubleMessage(char* buf, const double& val);

uint32_t encodeDoubleProfileMessage(char* buf, Profile<double>& val);

uint32_t encodeFloatMessage(char* buf, const float& val);

uint32_t encodeFloatProfileMessage(char* buf, Profile<float>& val);

uint32_t encodeBoolMessage(char* buf, const bool& val);

uint32_t encodeBoolProfileMessage(char* buf, Profile<bool>& val);

uint32_t encodeMatrixMessage(char* buf, const Matrix& val);

uint32_t encodeMatrixProfileMessage(char* buf, Profile<Matrix>& val);

uint32_t encodeQuatMessage(char* buf, const Quat& val);

uint32_t encodeQuatProfileMessage(char* buf, Profile<Quat>& val);

uint32_t calcProfileSize(Profile<int>& val);
uint32_t calcProfileSize(Profile<double>& val);
uint32_t calcProfileSize(Profile<float>& val);
uint32_t calcProfileSize(Profile<bool>& val);
uint32_t calcProfileSize(Profile<Matrix>& val);
uint32_t calcProfileSize(Profile<Quat>& val);

}}} // horizon::net::encode

#endif