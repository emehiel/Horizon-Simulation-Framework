#define _WINSOCKAPI_ // Fix for redefinition errors

#ifndef NETWORKEDSUBSYSTEM_H
#define NETWORKEDSUBSYSTEM_H

#include "horizon/net/encode/dataEncoder.h"
#include "horizon/net/sub/SubServerMessage.h"
#include "horizon/net/sockets/SocketClient.h"
#include "horizon/sub/dep/Dependencies.h"
#include "horizon/Subsystem.h"
#include "simParams.h"

using namespace horizon;
using namespace horizon::net::sub;
using namespace horizon::net::sockets;
using namespace horizon::net::encode;
using namespace horizon::geom;
using horizon::sub::dep::Dependencies;

namespace horizon {
	namespace net {
		namespace sub {

/**
 * A Subsystem that is provided by the Horizon Subsystem Server.
 */
class NetworkedSubsystem : public Subsystem {
private:
	SocketClient* server;
	string host;
	int port;
	string subName;

public:
	/**
	 * Constructor for NetworkedSubsystem.
	 * @param host The address of the Subsystem Server that will provide this Subsystem model.
	 * @param port The port that the Subsysem Server is running on.
	 * @param subName The name of the Subsystem.
	 */
	NetworkedSubsystem(string host, int port, string subName);

	virtual NetworkedSubsystem* clone() const;

	/** Destructor for NetworkedSubsystem. */
	~NetworkedSubsystem();

	virtual bool canPerform(const State* oldState, State* newState, const Task* task, Position* position, Environment* environment, NodeDependencies* dependencies) const;

	virtual bool canExtend(State* newState, Position* position, Environment* environment, const double evalToTime, NodeDependencies* dependencies) const;

	/**
	 * Tests whether the connection to the Subsystem Server is open.
	 * @return True if the connection is open, false otherwise.
	 */
	bool isOpen() const;

	bool processMessage_Task(DATA_HEADER_NS msg, const Task* task) const;

	bool processMessage_Target(DATA_HEADER_NS msg, const Target* target) const;

	bool processMessage_Sun(DATA_HEADER_NS msg, Sun* sun, Position* assetPos, Position* targetPos) const;

	bool processMessage_simParams(DATA_HEADER_NS msg) const;

	bool processMessage_Position(DATA_HEADER_NS msg, Position* assetPos, Position* targetPos) const;

	bool processMessage_State(DATA_HEADER_NS msg, State* state) const;

	bool processMessage_NodeDependencies(DATA_HEADER_NS msg, NodeDependencies* dependencies) const;
};

}}} // horizon::net::sub

#endif