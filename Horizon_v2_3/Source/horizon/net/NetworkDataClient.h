/**
 * This file contains the NetworkDataClient class.
 * @author Brian Butler
 */

#ifndef NETWORKDATACLIENT_H
#define NETWORKDATACLIENT_H

#include "horizon/util/Profile.h"
#include "horizon/util/Matrix.h"
#include "horizon/util/Quat.h"

#include "horizon/net/sockets/SocketClient.h"
#include "horizon/net/encode/dataEncoder.h"
#include "horizon/net/handler/HandlerType.h"

using namespace horizon::net::sockets;
using horizon::net::sockets::SocketException;
using namespace horizon::net::encode;
using namespace horizon::net::handler;
using namespace horizon::util;

namespace horizon {
	namespace net {
/**
 * This class handles communication to and from the data server.
 * @author Brian Butler
 */
class NetworkDataClient {
public:
	friend class Socket;

	/**
	 * Requests integer data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	int requestData_Int(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests integer profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Profile<int> requestData_IntProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Requests double data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	double requestData_Double(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests double profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Profile<double> requestData_DoubleProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Requests float data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	float requestData_Float(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests float profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Profile<float> requestData_FloatProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Requests boolean data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	bool requestData_Bool(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests boolean profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Profile<bool> requestData_BoolProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Requests Matrix data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Matrix requestData_Matrix(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests Matrix profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */	
	Profile<Matrix> requestData_MatrixProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Requests Quat data from the server.
	 * @param var The variable name that is requested.
	 * @time The time to retrieve the variable at.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Quat requestData_Quat(string var, double time, HandlerType type = ANY_DATA_HANDLER);
	
	/**
	 * Requests Quat profile data from the server.
	 * @param var The variable name that is requested.
	 * @start The starting time of the requested data.
	 * @start The ending time of the requested data.
	 * @type The type of DataHandler that should be used to process the data request.
	 * @return The requested data.
	 */
	Profile<Quat> requestData_QuatProfile(string var, double start, double end, HandlerType type = ANY_DATA_HANDLER);

	/**
	 * Sets the port to connect on.
	 * This method must be called before any instance of the NetworkDataClient is used.
	 * @param port The port to connect on.
	 */
	static void setPort(int port);
/**
	 * Sets the address to connect to.
	 * This method must be called before any instance of the NetworkDataClient is used.
	 * @param host The address to connect to.
	 */
	static void setHost(string host);

	/**
	 * Tests whether the connection is open.
	 * @return True if the connection is open, false otherwise.
	 */
	bool isOpen();

	/**
	 * Gets the instance of NetworkDataClient.
	 * @return A pointer to the NetworkDataClient instance.
	 */
	static NetworkDataClient* Instance();

	/**
	 * Connects to the server.
	 * The host and port must be set by using <CODE>setHost()</CODE>
	 * and <CODE>setPort()</CODE> prior to calling this method.
	 * @return True if the connection succeeds, false otherwise.
	 */
	static bool Connect();

	/**
	 * Connects to the server.
	 * @param host The host to connect to.
	 * @param port The port to connect to on the host.
	 * @return True if the connection succeeds, false otherwise.
	 */
	static bool Connect(string host, int port);

	/**
	 * Disconnects from the server.
	 * @return True if the disconnect was cleanly performed.
	 */
	static bool Disconnect();
protected:
	/** Default constructor for the NetworkDataClient class. */
	NetworkDataClient();
	/** Copy constructor for the NetworkDataClient class. */
	NetworkDataClient(const NetworkDataClient&);
	/** Copy assignment operator for the NetworkDataClient class. */
	NetworkDataClient& operator= (const NetworkDataClient&);
private:
	/** The default port to connect to on the server. */
	static const int DEFAULT_PORT = 10000;
	/** Flag to hold whether a connection to the server exists. */
	static bool isConnected;
	/** Pointer to the single instance of the NetworkDataClient class. */
	static NetworkDataClient* pinstance;
	/** The port on the server to connect to. */
	static int port;
	/** The address of the server to connect to. */
	static string host;
	/** A pointer to the SocketClient used to communicate to the server. */
	static SocketClient* server;
};

}} // end namespace horizon::net

#endif