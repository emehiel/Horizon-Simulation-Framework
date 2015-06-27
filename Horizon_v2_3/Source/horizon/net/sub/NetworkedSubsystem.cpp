#include "NetworkedSubsystem.h"

namespace horizon {
	namespace net {
		namespace sub {

NetworkedSubsystem::NetworkedSubsystem(string host, int port, string subName) : 
		Subsystem(subName),
		host(host),
		port(port),
		subName(subName),
		server(new SocketClient(host, port)) {}

NetworkedSubsystem* NetworkedSubsystem::clone() const {
	NetworkedSubsystem* newSS = new NetworkedSubsystem(this->host, this->port, this->subName);
	return newSS;
}

NetworkedSubsystem::~NetworkedSubsystem() {	delete server; }

bool NetworkedSubsystem::canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const {
	try {
		if(!isOpen()) return false;

		// First send the request to start to the server
		DATA_HEADER_NS startCanPerformMessage;
		startCanPerformMessage.type = REQUEST_ | CAN_PERFORM_;
		startCanPerformMessage.stubType = 0; // Not a stub fcn, set this param to 0
		// Encode the message into buffer with extra space for string to be sent
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(startCanPerformMessage, subName.length(), msgSize);
		// Get the pointer to string contents and copy into message buffer
		const char* strBuf = subName.c_str();
		memcpy(buf + msgSize - subName.length(), strBuf, subName.length());
		// Assign buffer to string, delete buffer, and send message
		string send;
		send.assign(buf, msgSize);
		server->SendBytes(send);

		// Now loop through and receive requests from the server
		DATA_HEADER_NS msg;
		while(1) {
			msg = server->ReadHeaderNS();
			if(msg.type ==  RETURN_ + CAN_PERFORM_)
				break;
			if(msg.type == REQUEST_ + TASK_) {
				processMessage_Task(msg, task);
			}
			if(msg.type == REQUEST_ + NODE_DEPENDENCIES_) {
				processMessage_NodeDependencies(msg, dependencies);
			}
			else if(msg.type == REQUEST_ + TARGET_) {
				processMessage_Target(msg, task->getTarget());
			}
			else if(msg.type == REQUEST_ + SUN_) {
				processMessage_Sun(msg, environment->getSun(), position, task->getTarget()->getPos());
			}
			else if(msg.type == REQUEST_ + SIM_PARAMS_) {
				processMessage_simParams(msg);
			}
			else if(msg.type == REQUEST_ + POSITION_) {
				processMessage_Position(msg, position, task->getTarget()->getPos());
			}
			else if(msg.type == REQUEST_ + EOMS_) {
				// First get which position the eoms is for
				// Read the args from the socket
				size_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
				char* args = new char[bufSize];
				server->ReadBytesIntoBuffer(args, bufSize);
				// Now read which position to use
				WHICH_POSITION whichPos;
				memcpy(&whichPos, args, sizeof(WHICH_POSITION));
				// Now read back index
				Matrix y = decodeMatrixMessageData(args + sizeof(WHICH_STATE));
				uint32_t matSize = 2*sizeof(unsigned) + y.getNumElems()*sizeof(double);
				// Now read time
				double time = decodeDoubleMessageData(args + sizeof(WHICH_STATE) + matSize);

				// Get the parent position
				Position* pos = NULL;
				if(whichPos == ASSET_POS)
					pos = position;
				else if(whichPos == TARGET_POS)
					pos = task->getTarget()->getPos();

				if(pos == NULL)
					return false;

				EOMS* eoms = pos->getEOMS();
				Matrix dy = (*eoms)(time, y);
				// Create the header to send back
				DATA_HEADER_NS sendHeader;				
				sendHeader.type = RETURN_ + EOMS_;
				sendHeader.stubType = CALL_OPERATOR;
				uint32_t msgSize = 0;
				// Calculate the size the Matrix will need in buffer
				uint32_t matrixSize = 2*sizeof(unsigned) + dy.getNumElems()*(sizeof(double));
				// Create buffer
				char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
				// Copy Matrix data in
				encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, dy);
				// Create the string to send back, send
				string sendString;
				sendString.assign(buf, msgSize);
				delete[] buf;
				server->SendBytes(sendString);
			}
			else if(msg.type == REQUEST_ + STATE_) {
				// First get which state and the back index to determine which state to use
				// Read the args from the socket
				size_t bufSize = sizeof(WHICH_STATE) + sizeof(int);
				char* args = new char[bufSize];
				server->ReadBytesIntoBuffer(args, bufSize);
				// Now read which position to use
				WHICH_STATE whichState;
				memcpy(&whichState, args, sizeof(WHICH_STATE));
				// Now read back index
				int backIndex;
				memcpy(&backIndex, args + sizeof(WHICH_STATE), sizeof(int));

				// Get the base state
				State* state = NULL;
				if(whichState == OLD_STATE)
					state = (State*)oldState;
				else if(whichState == NEW_STATE)
					state = (State*)newState;
				else {
					// The state is a dependencies state
					state = Dependencies::getState((int)whichState);
				}

				// Check null condition
				if(state == NULL)
					return false;

				// Iterate back getting prev state
				while(backIndex-- > 0) {
					state = (State*)state->getPrevious();
				}
				// Process the message
				processMessage_State(msg, state);
			}
		}
		return (bool)msg.stubType;
	}
	catch(SocketException& se) {
		cout << "Error calling NetworkedSubsystem " << subName << " at " << host << ":" << port << endl;
		return false;
	}
}

bool NetworkedSubsystem::canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* dependencies) const {
	try {
		if(!isOpen()) return false;

		// First send the request to start to the server
		DATA_HEADER_NS startCanPerformMessage;
		startCanPerformMessage.type = REQUEST_ | CAN_EXTEND_;
		startCanPerformMessage.stubType = 0; // Not a stub fcn, set this param to 0
		// Encode the message into buffer with extra space for string to be sent
		uint32_t msgSize = 0;
		uint32_t argsLen = subName.length() + sizeof(double);
		char* buf = encodeDataHeaderNSEx(startCanPerformMessage, argsLen, msgSize);
		// Get the pointer to string contents and copy into message buffer
		const char* strBuf = subName.c_str();
		// Copy string data in
		memcpy(buf + msgSize - argsLen, strBuf, subName.length());
		// Copy eval to time data in
		memcpy(buf + msgSize - sizeof(double), &evalToTime, sizeof(double));
		// Assign buffer to string, delete buffer, and send message
		string send;
		send.assign(buf, msgSize);
		server->SendBytes(send);

		// Now loop through and receive requests from the server
		DATA_HEADER_NS msg;
		while(1) {
			msg = server->ReadHeaderNS();
			if(msg.type ==  RETURN_ + CAN_EXTEND_)
				break;
			if(msg.type == REQUEST_ + NODE_DEPENDENCIES_) {
				processMessage_NodeDependencies(msg, dependencies);
			}
			else if(msg.type == REQUEST_ + SUN_) {
				processMessage_Sun(msg, environment->getSun(), position, NULL);
			}
			else if(msg.type == REQUEST_ + SIM_PARAMS_) {
				processMessage_simParams(msg);
			}
			else if(msg.type == REQUEST_ + POSITION_) {
				processMessage_Position(msg, position, NULL);
			}
			else if(msg.type == REQUEST_ + EOMS_) {
				// First get which position the eoms is for
				// Read the args from the socket
				size_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
				char* args = new char[bufSize];
				server->ReadBytesIntoBuffer(args, bufSize);
				// Now read which position to use
				WHICH_POSITION whichPos;
				memcpy(&whichPos, args, sizeof(WHICH_POSITION));
				// Now read back index
				Matrix y = decodeMatrixMessageData(args + sizeof(WHICH_STATE));
				uint32_t matSize = 2*sizeof(unsigned) + y.getNumElems()*sizeof(double);
				// Now read time
				double time = decodeDoubleMessageData(args + sizeof(WHICH_STATE) + matSize);

				// Get the parent position
				Position* pos = NULL;
				if(whichPos == ASSET_POS)
					pos = position;

				if(pos == NULL)
					return false;

				EOMS* eoms = pos->getEOMS();
				Matrix dy = (*eoms)(time, y);
				// Create the header to send back
				DATA_HEADER_NS sendHeader;				
				sendHeader.type = RETURN_ + EOMS_;
				sendHeader.stubType = CALL_OPERATOR;
				uint32_t msgSize = 0;
				// Calculate the size the Matrix will need in buffer
				uint32_t matrixSize = 2*sizeof(unsigned) + dy.getNumElems()*(sizeof(double));
				// Create buffer
				char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
				// Copy Matrix data in
				encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, dy);
				// Create the string to send back, send
				string sendString;
				sendString.assign(buf, msgSize);
				delete[] buf;
				server->SendBytes(sendString);
			}
			else if(msg.type == REQUEST_ + STATE_) {
				// First get which state and the back index to determine which state to use
				// Read the args from the socket
				size_t bufSize = sizeof(WHICH_STATE) + sizeof(int);
				char* args = new char[bufSize];
				server->ReadBytesIntoBuffer(args, bufSize);
				// Now read which position to use
				WHICH_STATE whichState;
				memcpy(&whichState, args, sizeof(WHICH_STATE));
				// Now read back index
				int backIndex;
				memcpy(&backIndex, args + sizeof(WHICH_STATE), sizeof(int));

				// Get the base state
				State* state = NULL;
				if(whichState == OLD_STATE)
					return false; // no oldState in canExtend
				else if(whichState == NEW_STATE)
					state = (State*)newState;
				else {
					// The state is a dependencies state
					state = Dependencies::getState((int)whichState);
				}

				// Check null condition
				if(state == NULL)
					return false;

				// Iterate back getting prev state
				while(backIndex-- > 0) {
					state = (State*)state->getPrevious();
				}
				// Process the message
				processMessage_State(msg, state);
			}
		}
		return (bool)msg.stubType;
	}
	catch(SocketException& se) {
		cout << "Error calling NetworkedSubsystem " << subName << " at " << host << ":" << port << endl;
		return false;
	}
}

bool NetworkedSubsystem::isOpen() const {
	// Create the header to send back
	DATA_HEADER_NS testSendHeader;				
	testSendHeader.type = CONN_TEST_;
	testSendHeader.stubType = 0;
	uint32_t msgSize = 0;
	// Create buffer
	char* buf = encodeDataHeaderNSEx(testSendHeader, 0, msgSize);
	// Create the string to send back, send
	string sendString;
	sendString.assign(buf, msgSize);
	delete[] buf;
	try {
		// Send the connection test
		server->SendBytes(sendString);
		// Receive connection test message back
		DATA_HEADER_NS testRecvHeader = server->ReadHeaderNS();
		if(testRecvHeader.type == CONN_TEST_ &&
			testRecvHeader.stubType == 0 &&
			testRecvHeader.size == 2*TYPE_BYTES + SIZE_BYTES)
		{
			return true;
		}
	}
	catch(SocketException& se) {
		cout << "NetworkedSubsystem connection not open: " << se.what() << endl;
	}
	return false;
}

bool NetworkedSubsystem::processMessage_NodeDependencies(DATA_HEADER_NS msg, NodeDependencies* dependencies) const {
	if(msg.stubType == CALL_INT_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<int> retProf = dependencies->callIntDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_INT_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeIntProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CALL_DOUBLE_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<double> retProf = dependencies->callDoubleDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_DOUBLE_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeDoubleProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CALL_FLOAT_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<float> retProf = dependencies->callFloatDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_FLOAT_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeFloatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CALL_BOOL_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<bool> retProf = dependencies->callBoolDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_BOOL_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeBoolProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CALL_MATRIX_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<Matrix> retProf = dependencies->callMatrixDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_MATRIX_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeMatrixProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CALL_QUAT_DEP) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		delete[] strBuf;
		// Get data
		Profile<Quat> retProf = dependencies->callQuatDependency(key);
		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + NODE_DEPENDENCIES_;
		sendHeader.stubType = CALL_QUAT_DEP;
		uint32_t msgSize = 0;
		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);
		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);
		// Assign value to buffer
		encodeQuatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_Task(DATA_HEADER_NS msg, const Task* task) const {
	if(msg.stubType == GET_TASK_TYPE) {
		// Get the data to send back
		string taskType = task->getType();
		const char* strBuf = taskType.c_str();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TASK_;
		sendHeader.stubType = GET_TASK_TYPE;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, taskType.length(), msgSize);
		memcpy(buf + 2*TYPE_BYTES + SIZE_BYTES, strBuf, taskType.length());

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_MAX_TIMES) {
		// Get the data to send back
		size_t maxTimes = task->getMaxTimesPerformable();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TASK_;
		sendHeader.stubType = GET_MAX_TIMES;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(size_t), msgSize);
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, maxTimes);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_Target(DATA_HEADER_NS msg, const Target* target) const {
	if(msg.stubType == GET_NAME) {
		// Get the data to send back
		string targetName = target->getName();
		const char* strBuf = targetName.c_str();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TARGET_;
		sendHeader.stubType = GET_NAME;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, targetName.length(), msgSize);
		memcpy(buf + 2*TYPE_BYTES + SIZE_BYTES, strBuf, targetName.length());

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_TARGET_TYPE) {
		// Get the data to send back
		string targetType = target->getType();
		const char* strBuf = targetType.c_str();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TARGET_;
		sendHeader.stubType = GET_TARGET_TYPE;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, targetType.length(), msgSize);
		memcpy(buf + 2*TYPE_BYTES + SIZE_BYTES, strBuf, targetType.length());

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_VALUE) {
		// Get the data to send back
		int value = target->getValue();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TARGET_;
		sendHeader.stubType = GET_VALUE;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(int), msgSize);
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, value);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_Sun(DATA_HEADER_NS msg, Sun* sun, Position* assetPos, Position* targetPos) const {
	if(msg.stubType == GET_SOLAR_CONSTANT) {
		// Get the data to send back
		int solarConstant = sun->getSolarConstant();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + TASK_;
		sendHeader.stubType = GET_MAX_TIMES;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(int), msgSize);
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, solarConstant);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_SUN_VEC) {
		// First read the args from the socket
		size_t bufSize = sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args, sizeof(double));

		// Call the method
		Matrix vec = sun->getEarSunVec(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + SUN_;
		sendHeader.stubType = GET_SUN_VEC;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + vec.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, vec);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == CAST_SHADOW) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use for pos 1
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		shadow_state state = sun->castShadowOnPos(posToUse, simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + SUN_;
		sendHeader.stubType = CAST_SHADOW;

		// Create buffer
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(int), msgSize);
		
		// Copy bool data in
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, (int)state);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_simParams(DATA_HEADER_NS msg) const {
	if(msg.stubType == GET_SIM_START_JD) {
		// Get the data to send back
		double simStartJD = simParams::SIMSTART_JD();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + SIM_PARAMS_;
		sendHeader.stubType = GET_SIM_START_JD;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, simStartJD);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_SIM_START_SECONDS) {
		// Get the data to send back
		double simStartSeconds = simParams::SIMSTART_SECONDS();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + SIM_PARAMS_;
		sendHeader.stubType = GET_SIM_START_SECONDS;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, simStartSeconds);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_SIM_END_SECONDS) {
		// Get the data to send back
		double simEndSeconds = simParams::SIMEND_SECONDS();

		// Create the header for the data to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + SIM_PARAMS_;
		sendHeader.stubType = GET_SIM_END_SECONDS;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, simEndSeconds);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_Position(DATA_HEADER_NS msg, Position* assetPos, Position* targetPos) const {
	if(msg.stubType == SET_POS_DATA_TIMESTEP) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read in the function argument
		double step;
		memcpy(&step, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		posToUse->setPosDataTimeStep(step);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = SET_POS_DATA_TIMESTEP;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_TYPE) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		string posType = posToUse->getType();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_TYPE;
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, posType.length(), msgSize);
		// Copy string data in
		const char* strBuf = posType.c_str();
		memcpy(buf + 2*TYPE_BYTES + SIZE_BYTES, strBuf, posType.length());
		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_IC) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix ic = posToUse->getIC();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_IC;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + ic.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, ic);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_STATE_ECI) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix state = posToUse->getStateECI(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_STATE_ECI;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + state.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, state);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_STATE_AT_TIME) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix state = posToUse->getStateAtTime(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_STATE_AT_TIME;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + state.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, state);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_POS_ECI) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix pos = posToUse->getPosECI(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_POS_ECI;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + pos.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, pos);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_VEL_ECI) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix vel = posToUse->getVelECI(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_VEL_ECI;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + vel.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, vel);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == GET_POS_LLA) {
		// First read the args from the socket
		size_t bufSize = sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		Matrix pos = posToUse->getPosLLA(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = GET_POS_LLA;
		uint32_t msgSize = 0;

		// Calculate the size the Matrix will need in buffer
		uint32_t matrixSize = 2*sizeof(unsigned) + pos.getNumElems() * (sizeof(double));

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, matrixSize, msgSize);
		
		// Copy Matrix data in
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, pos);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == HAS_LOS_TO_PD) {
		// First read the args from the socket
		size_t bufSize = 2*sizeof(WHICH_POSITION) + sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use for pos 1
		WHICH_POSITION whichPos1;
		memcpy(&whichPos1, args, sizeof(WHICH_POSITION));
		// Now read which position to use for pos 1
		WHICH_POSITION whichPos2;
		memcpy(&whichPos2, args + sizeof(WHICH_POSITION), sizeof(WHICH_POSITION));
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + 2*sizeof(WHICH_POSITION), sizeof(double));

		// Now get the position depending on args		
		Position* posToUse1 = NULL;
		if(whichPos1 == ASSET_POS)
			posToUse1 = assetPos;
		else if(whichPos1 == TARGET_POS)
			posToUse1 = targetPos;
		Position* posToUse2 = NULL;
		if(whichPos2 == ASSET_POS)
			posToUse2 = assetPos;
		else if(whichPos2 == TARGET_POS)
			posToUse2 = targetPos;

		// If error occured return false
		if(posToUse1 == NULL || posToUse2 == NULL)
			return false;

		// Call the method
		bool hasLOS = posToUse1->hasLOSTo(posToUse2, simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = HAS_LOS_TO_PD;

		// Create buffer
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(bool), msgSize);
		
		// Copy bool data in
		encodeBoolMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, hasLOS);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	else if(msg.stubType == HAS_LOS_TO_MD) {
		// First read the args from the socket
		size_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read which position to use for pos 1
		WHICH_POSITION whichPos;
		memcpy(&whichPos, args, sizeof(WHICH_POSITION));
		// Now read which position matrix to use for pos 2
		Matrix pos2 = decodeMatrixMessageData(args + sizeof(WHICH_POSITION));
		uint32_t matSize = 2*sizeof(unsigned) + pos2.getNumElems()*sizeof(double);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args + sizeof(WHICH_POSITION) + matSize, sizeof(double));

		// Now get the position depending on args		
		Position* posToUse = NULL;
		if(whichPos == ASSET_POS)
			posToUse = assetPos;
		else if(whichPos == TARGET_POS)
			posToUse = targetPos;

		// If error occured return false
		if(posToUse == NULL)
			return false;

		// Call the method
		bool hasLOS = posToUse->hasLOSTo(pos2, simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + POSITION_;
		sendHeader.stubType = HAS_LOS_TO_MD;

		// Create buffer
		uint32_t msgSize = 0;
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(bool), msgSize);
		
		// Copy bool data in
		encodeBoolMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, hasLOS);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

bool NetworkedSubsystem::processMessage_State(DATA_HEADER_NS msg, State* state) const {
	if(msg.stubType == HAS_PREVIOUS) {
		// Call the method
		bool hasPrev = (state->getPrevious() == NULL) ? false : true;

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = HAS_PREVIOUS;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(bool), msgSize);

		// Assign value to buffer
		encodeBoolMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, hasPrev);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}	
	else if(msg.stubType == SET_EVENT_START) {
		// First read the args from the socket
		size_t bufSize = sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args, sizeof(double));

		// Call the method
		state->setEventStart(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_EVENT_START;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_TASK_START) {
		// First read the args from the socket
		size_t bufSize = sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args, sizeof(double));

		// Call the method
		state->setTaskStart(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_TASK_START;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_TASK_END) {
		// First read the args from the socket
		size_t bufSize = sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args, sizeof(double));

		// Call the method
		state->setTaskEnd(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_TASK_END;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_EVENT_END) {
		// First read the args from the socket
		size_t bufSize = sizeof(double);
		char* args = new char[bufSize];
		server->ReadBytesIntoBuffer(args, bufSize);
		// Now read time argument
		double simTime;
		memcpy(&simTime, args, sizeof(double));

		// Call the method
		state->setEventEnd(simTime);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_EVENT_END;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_EVENT_START) {
		// Call the method
		double eventStart = state->getEventStart();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_EVENT_START;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);

		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, eventStart);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_TASK_START) {
		// Call the method
		double taskStart = state->getTaskStart();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_TASK_START;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);

		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, taskStart);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_TASK_END) {
		// Call the method
		double taskEnd = state->getTaskEnd();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_TASK_END;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);

		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, taskEnd);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_EVENT_END) {
		// Call the method
		double eventEnd = state->getEventEnd();

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_EVENT_END;
		uint32_t msgSize = 0;

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, sizeof(double), msgSize);

		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, eventEnd);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_INT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, int> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_INT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(int);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_DOUBLE) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, double> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_DOUBLE;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(double);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_FLOAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, float> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_FLOAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(float);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeFloatMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_BOOL) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, bool> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_BOOL;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(bool);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeBoolMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_MATRIX) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, Matrix> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_MATRIX;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + 2*sizeof(unsigned) + retPair.second.getNumElems()*sizeof(double);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_LAST_VALUE_QUAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] strBuf;
		// Get data
		pair<double, Quat> retPair = state->getLastValue(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_LAST_VALUE_QUAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + 4*sizeof(double);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeQuatMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_INT) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] buf;
		// Get data
		pair<double, int> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_INT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(int);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeIntMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_DOUBLE) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] buf;
		// Get data
		pair<double, double> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_DOUBLE;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(double);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_FLOAT) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] buf;
		// Get data
		pair<double, float> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_FLOAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(float);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeFloatMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_BOOL) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] buf;
		// Get data
		pair<double, bool> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_BOOL;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + sizeof(bool);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeBoolMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_MATRIX) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] buf;
		// Get data
		pair<double, Matrix> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_MATRIX;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + 2*sizeof(unsigned) + retPair.second.getNumElems()*sizeof(double);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeMatrixMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_VALUE_AT_TIME_QUAT) {
		// Get the remaining args size
		uint32_t argSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[argSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, argSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double), argSize - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] buf;
		// Get data
		pair<double, Quat> retPair = state->getValueAtTime(svk, time);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_VALUE_AT_TIME_QUAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t pairSize = sizeof(double) + 4*sizeof(double);

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, pairSize, msgSize);

		// Assign time to buffer
		encodeDoubleMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retPair.first);
		// Assign value to buffer
		encodeQuatMessage(buf + 2*TYPE_BYTES + SIZE_BYTES + sizeof(double), retPair.second);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_INT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] strBuf;
		// Get data
		Profile<int> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_INT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeIntProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_INT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] strBuf;
		// Get data
		Profile<int> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_INT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeIntProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_DOUBLE) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] strBuf;
		// Get data
		Profile<double> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_DOUBLE;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeDoubleProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_DOUBLE) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] strBuf;
		// Get data
		Profile<double> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_DOUBLE;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeDoubleProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_FLOAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] strBuf;
		// Get data
		Profile<float> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_FLOAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeFloatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_FLOAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] strBuf;
		// Get data
		Profile<float> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_FLOAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeFloatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_BOOL) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] strBuf;
		// Get data
		Profile<bool> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_BOOL;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeBoolProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_BOOL) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] strBuf;
		// Get data
		Profile<bool> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_BOOL;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeBoolProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_MATRIX) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] strBuf;
		// Get data
		Profile<Matrix> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_MATRIX;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeMatrixProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_MATRIX) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] strBuf;
		// Get data
		Profile<Matrix> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_MATRIX;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeMatrixProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_PROFILE_QUAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] strBuf;
		// Get data
		Profile<Quat> retProf = state->getProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_PROFILE_QUAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeQuatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == GET_FULL_PROFILE_QUAT) {
		// Get the remaining string size
		uint32_t strSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* strBuf = new char[strSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(strBuf, strSize);
		// Create string from buffer
		string key;
		key.assign(strBuf, strSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] strBuf;
		// Get data
		Profile<Quat> retProf = state->getFullProfile(svk);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = GET_FULL_PROFILE_QUAT;
		uint32_t msgSize = 0;

		// Calculate the size the profile requires
		uint32_t profSize = calcProfileSize(retProf);

		// Create buffer
		char* buf = encodeDataHeaderNSEx(sendHeader, profSize, msgSize);

		// Assign value to buffer
		encodeQuatProfileMessage(buf + 2*TYPE_BYTES + SIZE_BYTES, retProf);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_INT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		int val = decodeIntMessageData(buf + sizeof(double));

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + sizeof(int), bufSize - sizeof(double) - sizeof(int));
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_INT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_DOUBLE) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		double val = decodeDoubleMessageData(buf + sizeof(double));

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + sizeof(double), bufSize - sizeof(double) - sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_DOUBLE;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_FLOAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		float val = decodeFloatMessageData(buf + sizeof(double));

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + sizeof(float), bufSize - sizeof(double) - sizeof(float));
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_FLOAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_BOOL) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		bool val = decodeBoolMessageData(buf + sizeof(double));

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + sizeof(bool), bufSize - sizeof(double) - sizeof(bool));
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_BOOL;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_MATRIX) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		Matrix val = decodeMatrixMessageData(buf + sizeof(double));
		uint32_t matSize = 2*sizeof(unsigned) + val.getNumElems()*sizeof(double);

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + matSize, bufSize - sizeof(double) - matSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_MATRIX;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PAIR_QUAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read time from buffer
		double time = decodeDoubleMessageData(buf);
		// Read value from buffer
		Quat val = decodeQuatMessageData(buf + sizeof(double));

		// Create string from buffer
		string key;
		key.assign(buf + sizeof(double) + 4*sizeof(double), bufSize - sizeof(double) - 4*sizeof(double));
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, make_pair(time, val));

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PAIR_QUAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_INT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<int> val = decodeIntProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_INT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_DOUBLE) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<double> val = decodeDoubleProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_DOUBLE;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_FLOAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<float> val = decodeFloatProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_FLOAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_BOOL) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<bool> val = decodeBoolProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_BOOL;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_MATRIX) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<Matrix> val = decodeMatrixProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_MATRIX;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == ADD_VALUE_PROF_QUAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<Quat> val = decodeQuatProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] buf;
		// Add data
		state->addValue(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = ADD_VALUE_PROF_QUAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_INT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<int> val = decodeIntProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<int> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_INT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_DOUBLE) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<double> val = decodeDoubleProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<double> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_DOUBLE;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_FLOAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<float> val = decodeFloatProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<float> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_FLOAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_BOOL) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<bool> val = decodeBoolProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<bool> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_BOOL;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_MATRIX) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<Matrix> val = decodeMatrixProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Matrix> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_MATRIX;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
	if(msg.stubType == SET_PROF_QUAT) {
		// Get the remaining buffer size
		uint32_t bufSize = msg.size - (2*TYPE_BYTES + SIZE_BYTES + sizeof(WHICH_STATE) + sizeof(int));
		// Initialize buffer to hold string
		char* buf = new char[bufSize];
		// Read socket into buffer
		server->ReadBytesIntoBuffer(buf, bufSize);
		// Read Profile from buffer
		Profile<Quat> val = decodeQuatProfileMessageData(buf);
		uint32_t profSize = calcProfileSize(val);

		// Create string from buffer
		string key;
		key.assign(buf + profSize, bufSize - profSize);
		// Create StateVarKey, delete buffer
		StateVarKey<Quat> svk(key);
		delete[] buf;
		// Set Profile
		state->setProfile(svk, val);

		// Create the header to send back
		DATA_HEADER_NS sendHeader;				
		sendHeader.type = RETURN_ + STATE_;
		sendHeader.stubType = SET_PROF_QUAT;
		uint32_t msgSize = 0;

		// Create buffer
		buf = encodeDataHeaderNSEx(sendHeader, 0, msgSize);

		// Create the string to send back, send
		string sendString;
		sendString.assign(buf, msgSize);
		delete[] buf;
		server->SendBytes(sendString);
		return true;
	}
}

}}} // horizon::net::sub