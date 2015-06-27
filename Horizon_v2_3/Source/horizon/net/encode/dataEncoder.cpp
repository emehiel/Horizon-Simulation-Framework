#include "dataEncoder.h"

namespace horizon {
	namespace net {
		namespace encode {

DATA_TYPE getDataType(const DATA_HEADER& header) {
	BYTE pFlagRemoved = (header.type & 0x7F);
	if(pFlagRemoved & 0x01)
		return _INT;
	else if(pFlagRemoved & 0x02)
		return _DOUBLE;
	else if(pFlagRemoved & 0x04)
		return _FLOAT;
	else if(pFlagRemoved & 0x08)
		return _BOOL;
	else if(pFlagRemoved & 0x10)
		return _MATRIX;
	else if(pFlagRemoved & 0x20)
		return _QUAT;
	else
		return _ERROR;
}

varType DATA_TYPE_to_varType(DATA_TYPE type) {
	switch(type) {
		case _INT:
			return INT_; break;
		case _DOUBLE:
			return DOUBLE_; break;
		case _FLOAT:
			return FLOAT_; break;
		case _BOOL:
			return BOOL_; break;
		case _MATRIX:
			return MATRIX_; break;
		case _QUAT:
			return QUAT_; break;
		case _ERROR:
			return ERROR_; break;
		default:
			return ERROR_; break;
	}
}

bool isDataType(const DATA_HEADER& header, const DATA_TYPE& type) {
	BYTE pmFlagsRemoved = (header.type & 0x3F);
	return (pmFlagsRemoved & type)!=0;
}

bool isProfile(const DATA_HEADER& header) {
	return (header.type & 0x80)!=0;
}

bool isConnectionTest(const DATA_HEADER& header) {
	return (header.type & (_MSG | _YES_TEST)) == (_MSG | _YES_TEST);
}

bool isMessage(const DATA_HEADER& header) {
	return (header.type & 0x40)!=0;
}

string createConnectionTestMessage() {
	BYTE type = _MSG | _YES_TEST;
	uint32_t size = TYPE_BYTES + SIZE_BYTES;	
	char* buf = new char[size];
	memcpy(buf, &type, TYPE_BYTES);
	memcpy(buf + TYPE_BYTES, &size, SIZE_BYTES);
	string out;
	out.assign(buf, size);
	delete[] buf;
	return out;
}

string encodeMessage(MSG_DATA& msg) {
	// Calculate the size of the message
	uint32_t msgSize = TYPE_BYTES + SIZE_BYTES + sizeof(uint32_t) + 2*sizeof(double) + msg.var.length();
	// Initialize buffer
	char* buf = new char[msgSize];
	// Copy type
	memcpy(buf, &msg.header.type, TYPE_BYTES);
	// Copy size
	memcpy(buf + TYPE_BYTES, &msg.header.size, SIZE_BYTES);
	// Copy handler type
	memcpy(buf + TYPE_BYTES + SIZE_BYTES, &msg.handlerType, sizeof(uint32_t));
	// Copy start
	memcpy(buf + TYPE_BYTES + SIZE_BYTES + sizeof(uint32_t), &msg.start, sizeof(double));
	// Copy end
	memcpy(buf + TYPE_BYTES + SIZE_BYTES + sizeof(uint32_t) + sizeof(double), &msg.end, sizeof(double));
	// Get string data
	const char* strData = msg.var.c_str();
	// Copy var
	memcpy(buf + TYPE_BYTES + SIZE_BYTES + sizeof(uint32_t) + 2*sizeof(double), strData, msg.var.length());
	// Create output string
	string out;
	// Assign contents of buffer to string
	out.assign(buf, msgSize);
	delete[] buf;
	return out;
}

int decodeIntMessageData(const char* buf) {
	// Variable to hold data
	int val;
	// Copy value
	memcpy(&val, buf, sizeof(uint32_t));
	return val;
}

Profile<int> decodeIntProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	unsigned numPoints;
	memcpy(&numPoints, buf, sizeof(unsigned));
	// Create map to hold data
	map<double, int> data;
	// Loop through and read data
	double t;
	int y;
	for(size_t i = 0; i < numPoints; i++) {
		memcpy(&t, buf + sizeof(unsigned) + i*(sizeof(double) + sizeof(int)), sizeof(double));
		memcpy(&y, buf + sizeof(unsigned) + (i+1)*sizeof(double) + i*sizeof(int), sizeof(int));
		data.insert(make_pair(t,y));
	}
	// Return Profile constructed from data
	return Profile<int> (data);
}

double decodeDoubleMessageData(const char* buf) {
	// Variable to hold data
	double val;
	// Copy value
	memcpy(&val, buf, sizeof(double));
	return val;
}

Profile<double> decodeDoubleProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	unsigned numPoints;
	memcpy(&numPoints, buf, sizeof(unsigned));
	// Create map to hold data
	map<double, double> data;
	// Loop through and read data
	double t;
	double y;
	for(size_t i = 0; i < numPoints; i++) {
		memcpy(&t, buf + sizeof(unsigned) + i*(sizeof(double) + sizeof(double)), sizeof(double));
		memcpy(&y, buf + sizeof(unsigned) + (i+1)*sizeof(double) + i*sizeof(double), sizeof(double));
		data.insert(make_pair(t,y));
	}
	// Return Profile constructed from data
	return Profile<double> (data);
}

float decodeFloatMessageData(const char* buf) {
	// Variable to hold data
	float val;
	// Copy value
	memcpy(&val, buf, sizeof(float));
	return val;
}

Profile<float> decodeFloatProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	unsigned numPoints;
	memcpy(&numPoints, buf, sizeof(unsigned));
	// Create map to hold data
	map<double, float> data;
	// Loop through and read data
	double t;
	float y;
	for(size_t i = 0; i < numPoints; i++) {
		memcpy(&t, buf + sizeof(unsigned) + i*(sizeof(double) + sizeof(float)), sizeof(double));
		memcpy(&y, buf + sizeof(unsigned) + (i+1)*sizeof(double) + i*sizeof(float), sizeof(float));
		data.insert(make_pair(t,y));
	}
	// Return Profile constructed from data
	return Profile<float> (data);
}

bool decodeBoolMessageData(const char* buf) {
	// Variable to hold data
	bool val;
	// Copy value
	memcpy(&val, buf, sizeof(bool));
	return val;
}

Profile<bool> decodeBoolProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	unsigned numPoints;
	memcpy(&numPoints, buf, sizeof(unsigned));
	// Create map to hold data
	map<double, bool> data;
	// Loop through and read data
	double t;
	bool y;
	for(size_t i = 0; i < numPoints; i++) {
		memcpy(&t, buf + sizeof(unsigned) + i*(sizeof(double) + sizeof(bool)), sizeof(double));
		memcpy(&y, buf + sizeof(unsigned) + (i+1)*sizeof(double) + i*sizeof(bool), sizeof(bool));
		data.insert(make_pair(t,y));
	}
	// Return Profile constructed from data
	return Profile<bool> (data);
}

Matrix decodeMatrixMessageData(const char* buf) {
	// Now copy number of rows and cols
	unsigned rc[2];
	memcpy(&rc, buf, 2*sizeof(unsigned));
	// Dynamically initialize memory to hold array values
	double* vals = new double[rc[0]*rc[1]];
	// Copy values into memory
	memcpy(vals, buf + 2*sizeof(unsigned), rc[0]*rc[1]*sizeof(double));
	// Create the Matrix
	Matrix m = Matrix(rc[0], rc[1], vals);
	delete[] vals;
	return m;
}

Profile<Matrix> decodeMatrixProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	uint32_t numPoints;
	memcpy(&numPoints, buf, sizeof(uint32_t));
	// Create map to hold data
	map<double, Matrix> data;
	// Loop through and read data
	double t;
	size_t offset = 0;
	unsigned rc[2];	
	for(size_t i = 0; i < numPoints; i++) {
		// Get the time information
		memcpy(&t, buf + sizeof(uint32_t) + offset, sizeof(double));
		// Get the row and column information
		memcpy(&rc, buf + sizeof(uint32_t) + sizeof(double) + offset, 2*sizeof(unsigned));
		// Dynamically initialize memory to hold array values
		double* vals = new double[rc[0]*rc[1]];
		// Copy values into memory
		memcpy(vals, buf + sizeof(uint32_t) + sizeof(double) + offset + 2*sizeof(unsigned), rc[0]*rc[1]*sizeof(double));
		// Create the Matrix
		Matrix m = Matrix(rc[0], rc[1], vals);
		// Update offset, delete memory
		offset += sizeof(double) + 2*sizeof(unsigned) + rc[0]*rc[1]*sizeof(double);
		delete[] vals;
		data.insert(make_pair(t, m));
	}
	// Return Profile constructed from data
	return Profile<Matrix> (data);
}

Quat decodeQuatMessageData(const char* buf) {
	// Variables to hold data
	double vals[4];
	// Copy value
	memcpy(&vals, buf, 4*sizeof(double));
	return Quat(vals[0], vals[1], vals[2], vals[3]);
}

Profile<Quat> decodeQuatProfileMessageData(const char* buf) {
	// Now get the number of points in the Profile
	unsigned numPoints;
	memcpy(&numPoints, buf, sizeof(unsigned));
	// Create map to hold data
	map<double, Quat> data;
	// Loop through and read data
	double t;
	double vals[4];
	for(size_t i = 0; i < numPoints; i++) {
		memcpy(&t, buf + sizeof(unsigned) + i*(sizeof(double) + 4*sizeof(double)), sizeof(double));
		// Copy value
		memcpy(&vals, buf + sizeof(unsigned) + (i+1)*sizeof(double) + i*4*sizeof(double), 4*sizeof(double));
		data.insert(make_pair(t,Quat(vals[0], vals[1], vals[2], vals[3])));
	}
	// Return Profile constructed from data
	return Profile<Quat> (data);
}

uint32_t encodeIntMessage(char* buf, const uint32_t& val) {
	uint32_t msgSize = sizeof(uint32_t);
	// Copy value
	memcpy(buf, &val, sizeof(uint32_t));
	return msgSize;
}

uint32_t encodeIntProfileMessage(char* buf, Profile<int>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, int> data = val.getProfileData();
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(int)));
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	int i = 0;
	for(map<double, int>::iterator mIt = data.begin(); mIt != data.end(); mIt++, i++) {
		memcpy(buf + sizeof(uint32_t) + i*(sizeof(double) + sizeof(int)), &(mIt->first), sizeof(double));
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + i*sizeof(int), &(mIt->second), sizeof(int));
	}
	return msgSize;
}

uint32_t encodeDoubleMessage(char* buf, const double& val) {
	// Calculate total size in bytes
	uint32_t msgSize = sizeof(double);
	// Copy value
	memcpy(buf, &val, sizeof(double));
	return msgSize;
}

uint32_t encodeDoubleProfileMessage(char* buf, Profile<double>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, double> data = val.getProfileData();
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(double)));
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	int i = 0;
	for(map<double, double>::iterator mIt = data.begin(); mIt != data.end(); mIt++, i++) {
		memcpy(buf + sizeof(uint32_t) + i*(sizeof(double) + sizeof(double)), &(mIt->first), sizeof(double));
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + i*sizeof(double), &(mIt->second), sizeof(double));
	}
	return msgSize;
}

uint32_t encodeFloatMessage(char* buf, const float& val) {
	// Calculate total size in bytes
	uint32_t msgSize = sizeof(float);
	// Copy value
	memcpy(buf, &val, sizeof(float));
	return msgSize;
}

uint32_t encodeFloatProfileMessage(char* buf, Profile<float>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, float> data = val.getProfileData();
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(float)));
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	int i = 0;
	for(map<double, float>::iterator mIt = data.begin(); mIt != data.end(); mIt++, i++) {
		memcpy(buf + sizeof(uint32_t) + i*(sizeof(double) + sizeof(float)), &(mIt->first), sizeof(double));
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + i*sizeof(float), &(mIt->second), sizeof(float));
	}
	return msgSize;
}

uint32_t encodeBoolMessage(char* buf, const bool& val) {
	// Calculate total size in bytes
	uint32_t msgSize = sizeof(bool);
	// Copy value
	memcpy(buf, &val, sizeof(bool));
	return msgSize;
}

uint32_t encodeBoolProfileMessage(char* buf, Profile<bool>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, bool> data = val.getProfileData();
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(bool)));
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	int i = 0;
	for(map<double, bool>::iterator mIt = data.begin(); mIt != data.end(); mIt++, i++) {
		memcpy(buf + sizeof(uint32_t) + i*(sizeof(double) + sizeof(bool)), &(mIt->first), sizeof(double));
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + i*sizeof(bool), &(mIt->second), sizeof(bool));
	}
	return msgSize;
}

uint32_t encodeMatrixMessage(char* buf, const Matrix& val) {
	// Calculate total size in bytes
	// Matrix needs 1*sizeof(unsigned) for row, 1*sizeof(unsigned) for col, numElems()*sizeof(double)
	uint32_t msgSize = 2*sizeof(unsigned) + val.getNumElems()*sizeof(double);
	// Copy rows and columns
	unsigned rows = val.getNumRows();
	unsigned cols = val.getNumCols();
	memcpy(buf, &rows, sizeof(unsigned));
	memcpy(buf + sizeof(unsigned), &cols, sizeof(unsigned));
	// Copy data
	int i = 0;
	const deque<double> data = val.getData();
	for(deque<double>::const_iterator mIt = data.begin(); mIt != data.end(); i++, mIt++) {
		memcpy(buf + 2*sizeof(unsigned) + i*sizeof(double), &(*mIt), sizeof(double));
	}
	return msgSize;
}

uint32_t encodeMatrixProfileMessage(char* buf, Profile<Matrix>& val) {
	// Get the number of points in the profile
	uint32_t numPoints = (uint32_t)val.size();
	// Get the data in the profile
	map<double, Matrix> data = val.getProfileData();
	// Get the size of all Matrices
	size_t mSize = 0;
	for(map<double, Matrix>::iterator mIt = data.begin(); mIt != data.end(); mIt++)
		mSize += sizeof(double) + 2*sizeof(unsigned) + (mIt->second).getNumElems()*sizeof(double);
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + mSize);
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	size_t offset = 0;
	for(map<double, Matrix>::iterator mIt = data.begin(); mIt != data.end(); mIt++) {
		// Copy time information
		memcpy(buf + sizeof(uint32_t) + offset, &(mIt->first), sizeof(double));
		// Get rows and cols of current Matrix
		unsigned rows = (mIt->second).getNumRows();
		unsigned cols = (mIt->second).getNumCols();
		// Copy row and col info
		memcpy(buf + sizeof(uint32_t) + offset + sizeof(double), &rows, sizeof(unsigned));
		memcpy(buf + sizeof(uint32_t) + offset + sizeof(double) + sizeof(unsigned), &cols, sizeof(unsigned));		
		// Loop through data within matrix
		int i = 0;
		const deque<double> data = (mIt->second).getData();
		for(deque<double>::const_iterator mIt = data.begin(); mIt != data.end(); i++, mIt++) {
			memcpy(buf + sizeof(uint32_t) + offset + sizeof(double) + 2*sizeof(unsigned) + i*sizeof(double), &(*mIt), sizeof(double));
		}
		// Update offset
		offset += sizeof(double) + 2*sizeof(unsigned) + rows*cols*sizeof(double);
	}
	return msgSize;
}

uint32_t encodeQuatMessage(char* buf, const Quat& val) {
	// Calculate total size in bytes, a Quat needs 4 double values
	uint32_t msgSize = 4*sizeof(double);
	// Copy values
	double q = val.comp_q0();
	memcpy(buf, &q, sizeof(double));
	q = val.comp_q1();
	memcpy(buf + 1*sizeof(double), &q, sizeof(double));
	q = val.comp_q2();
	memcpy(buf + 2*sizeof(double), &q, sizeof(double));
	q = val.comp_q3();
	memcpy(buf + 3*sizeof(double), &q, sizeof(double));
	return msgSize;
}

uint32_t encodeQuatProfileMessage(char* buf, Profile<Quat>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, Quat> data = val.getProfileData();
	// Calculate total size in bytes
	uint32_t msgSize = (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + 4*sizeof(double)));
	// Copy number of points
	memcpy(buf, &numPoints, sizeof(uint32_t));
	// Loop through and copy data
	int i = 0;
	double q;
	for(map<double, Quat>::iterator mIt = data.begin(); mIt != data.end(); mIt++, i++) {
		memcpy(buf + sizeof(uint32_t) + i*(sizeof(double) + 4*sizeof(double)), &(mIt->first), sizeof(double));
		q = (mIt->second).comp_q0();
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + i*4*sizeof(double), &q, sizeof(double));
		q = (mIt->second).comp_q1();
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + (i*4+1)*sizeof(double), &q, sizeof(double));
		q = (mIt->second).comp_q2();
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + (i*4+2)*sizeof(double), &q, sizeof(double));
		q = (mIt->second).comp_q3();
		memcpy(buf + sizeof(uint32_t) + (i+1)*sizeof(double) + (i*4+3)*sizeof(double), &q, sizeof(double));
	}
	return msgSize;
}

uint32_t calcProfileSize(Profile<int>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, int> data = val.getProfileData();
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(int)));
}

uint32_t calcProfileSize(Profile<double>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, double> data = val.getProfileData();
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(double)));
}

uint32_t calcProfileSize(Profile<float>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, float> data = val.getProfileData();
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(float)));
}

uint32_t calcProfileSize(Profile<bool>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, bool> data = val.getProfileData();
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + sizeof(bool)));
}

uint32_t calcProfileSize(Profile<Matrix>& val) {
	// Get the number of points in the profile
	uint32_t numPoints = (uint32_t)val.size();
	// Get the data in the profile
	map<double, Matrix> data = val.getProfileData();
	// Get the size of all Matrices
	size_t mSize = 0;
	for(map<double, Matrix>::iterator mIt = data.begin(); mIt != data.end(); mIt++)
		mSize += sizeof(double) + 2*sizeof(unsigned) + (mIt->second).getNumElems()*sizeof(double);
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + mSize);
}

uint32_t calcProfileSize(Profile<Quat>& val) {
	// Get the number of points in the profile
	size_t numPoints = val.size();
	// Get the data in the profile
	map<double, Quat> data = val.getProfileData();
	// Calculate total size in bytes
	return (uint32_t)(sizeof(uint32_t) + numPoints*(sizeof(double) + 4*sizeof(double)));
}

}}} // horizon::net::encode