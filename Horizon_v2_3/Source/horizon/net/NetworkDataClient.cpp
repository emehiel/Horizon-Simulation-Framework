#include "NetworkDataClient.h"

namespace horizon {
	namespace net {

NetworkDataClient* NetworkDataClient::pinstance = 0;// initialize pointer

bool NetworkDataClient::isConnected = false;

int NetworkDataClient::port = DEFAULT_PORT;

string NetworkDataClient::host = "127.0.0.1";

SocketClient* NetworkDataClient::server = NULL;

int NetworkDataClient::requestData_Int(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _INT;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	int ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		int ret = NetworkDataClient::server->ReceiveMessage_Int();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
		return int();
	}
	return ret;
}

Profile<int> NetworkDataClient::requestData_IntProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _INT | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<int> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_IntProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

double NetworkDataClient::requestData_Double(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _DOUBLE;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	double ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_Double();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Profile<double> NetworkDataClient::requestData_DoubleProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _DOUBLE | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<double> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_DoubleProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

float NetworkDataClient::requestData_Float(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _FLOAT;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	float ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_Float();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Profile<float> NetworkDataClient::requestData_FloatProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _FLOAT | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<float> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_FloatProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

bool NetworkDataClient::requestData_Bool(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _BOOL;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	bool ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_Bool();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Profile<bool> NetworkDataClient::requestData_BoolProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _BOOL | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<bool> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_BoolProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Matrix NetworkDataClient::requestData_Matrix(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _MATRIX;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Matrix ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		Matrix ret = NetworkDataClient::server->ReceiveMessage_Matrix();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Profile<Matrix> NetworkDataClient::requestData_MatrixProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _MATRIX | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<Matrix> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_MatrixProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Quat NetworkDataClient::requestData_Quat(string var, double time, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _QUAT;
	msg.handlerType = type;
	msg.start = time;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Quat ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_Quat();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

Profile<Quat> NetworkDataClient::requestData_QuatProfile(string var, double start, double end, HandlerType type) {
	MSG_DATA msg;
	msg.header.type = _MSG | _QUAT | _PROF;
	msg.handlerType = type;
	msg.start = start;
	msg.end = end;
	msg.var = var;
	msg.header.size = TYPE_BYTES + SIZE_BYTES + 2*sizeof(double) + sizeof(uint32_t) + var.length();
	Profile<Quat> ret;
	try {
		NetworkDataClient::server->SendBytes(encodeMessage(msg));
		ret = NetworkDataClient::server->ReceiveMessage_QuatProfile();
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
		Disconnect();
	}
	return ret;
}

void NetworkDataClient::setPort(int port) {
	NetworkDataClient::port = port;
}

void NetworkDataClient::setHost(string host) {
	NetworkDataClient::host = host;
}

bool NetworkDataClient::isOpen() {
	if(!isConnected)
		return false;
	try {
		server->SendBytes(createConnectionTestMessage());
		DATA_HEADER header = server->ReadHeader();
		if(isConnectionTest(header))
			return true;
	}
	catch(SocketException& se) {
		return false;
	}
	return false;
}

bool NetworkDataClient::Connect() {
	try {
		server = new SocketClient(host, port);
		isConnected = true;
		return true;
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
	}
	return false;
}

bool NetworkDataClient::Connect(string host, int port) {
	NetworkDataClient::host = host;
	NetworkDataClient::port = port;
	return NetworkDataClient::Connect();

}

bool NetworkDataClient::Disconnect() {
	try {
		isConnected = false;
		delete server;
		return true;
	}
	catch(SocketException& se) {
		cout << se.what() << endl;
	}
	return false;
}


NetworkDataClient* NetworkDataClient::Instance() {
	if (pinstance == 0) {  // is it the first call?
		pinstance = new NetworkDataClient();
    }
    return pinstance; // address of sole instance
}

NetworkDataClient::NetworkDataClient() {}

}} // end namespace horizon::net