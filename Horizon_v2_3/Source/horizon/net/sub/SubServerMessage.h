#define _WINSOCKAPI_ // Fix for redefinition errors

#ifndef SUBSERVERMESSAGE_H
#define SUBSERVERMESSAGE_H

#include "horizon/net/encode/dataEncoder.h"

namespace horizon {
	namespace net {
		namespace sub {

/**
 * Enumeration for the stub type of a message.
 */
enum MESSAGE_STUB_TYPE {
	EOMS_				= 1,
	ENVIRONMENT_		= 2,
	NODE_DEPENDENCIES_	= 3,
	POSITION_			= 4,
	STATE_				= 5,
	SUN_				= 6,
	TARGET_				= 7,
	TASK_				= 8,
	SIM_PARAMS_			= 9,
	/** Message is a request to start evaluating the Subsystem's canPerform method. */
	CAN_PERFORM_		= 10,
	/** Message is a request to start evaluating the Subsystem's canExtend method. */
	CAN_EXTEND_			= 11
};

/**
 * Enumeration for the type of message.
 */
enum MESSAGE_TYPE {
	/** Message is a return value from a stub function call. */
	RETURN_				= 64,
	/** Message is a stub function call. */
	REQUEST_			= 128,
	/** Message is a connection test. */
	CONN_TEST_			= 255
};

/**
 * Enumeration for which Position a Position stub represents.
 */
enum WHICH_POSITION {
	/** Stub represents the Position of the current Asset. */
	ASSET_POS			= 1,
	/** Stub represents the Position of the current Target. */
	TARGET_POS			= 2
};

/**
 * Enumeration for which State a State stub represents.
 */
enum WHICH_STATE {
	/** Stub represents the previous State. */
	OLD_STATE			= -1,
	/** Stub represents the current State. */
	NEW_STATE			= -2
};

/** Which method of EOMS stub class is called. */
enum EOMS_STUB_FCN {
	CALL_OPERATOR = 1
};

/** Which method of NodeDependencies stub class is called. */
enum NODE_DEPENDENCIES_STUB_FCN {
	CALL_INT_DEP		= 1,
	CALL_DOUBLE_DEP		= 2,
	CALL_FLOAT_DEP		= 3,
	CALL_BOOL_DEP		= 4,
	CALL_MATRIX_DEP		= 5,
	CALL_QUAT_DEP		= 6
};

/** Which method of Position stub class is called. */
enum POSITION_STUB_FCN {
	SET_POS_DATA_TIMESTEP	= 1,
	GET_TYPE				= 2,
	GET_IC					= 3,
	GET_STATE_ECI			= 4,
	GET_STATE_AT_TIME		= 5,
	GET_POS_ECI				= 6,
	GET_VEL_ECI				= 7,
	GET_POS_LLA				= 8,
	HAS_LOS_TO_PD			= 9,
	HAS_LOS_TO_MD			= 10
};

/** Which method of State stub class is called. */
enum STATE_STUB_FCN {
	HAS_PREVIOUS			= 1,
	SET_EVENT_START			= 2,
	SET_TASK_START			= 3,
	SET_TASK_END			= 4,
	SET_EVENT_END			= 5,
	GET_EVENT_START			= 6,
	GET_TASK_START			= 7,
	GET_TASK_END			= 8,
	GET_EVENT_END			= 9,
	GET_PROFILE_INT			= 10,
	GET_PROFILE_DOUBLE		= 11,
	GET_PROFILE_FLOAT		= 12,
	GET_PROFILE_BOOL		= 13,
	GET_PROFILE_MATRIX		= 14,
	GET_PROFILE_QUAT		= 15,
	GET_FULL_PROFILE_INT	= 16,
	GET_FULL_PROFILE_DOUBLE	= 17,
	GET_FULL_PROFILE_FLOAT	= 18,
	GET_FULL_PROFILE_BOOL	= 19,
	GET_FULL_PROFILE_MATRIX	= 20,
	GET_FULL_PROFILE_QUAT	= 21,
	GET_LAST_VALUE_INT		= 22,
	GET_LAST_VALUE_DOUBLE	= 23,
	GET_LAST_VALUE_FLOAT	= 24,
	GET_LAST_VALUE_BOOL		= 25,
	GET_LAST_VALUE_MATRIX	= 26,
	GET_LAST_VALUE_QUAT		= 27,
	GET_VALUE_AT_TIME_INT	= 28,
	GET_VALUE_AT_TIME_DOUBLE= 29,
	GET_VALUE_AT_TIME_FLOAT	= 30,
	GET_VALUE_AT_TIME_BOOL	= 31,
	GET_VALUE_AT_TIME_MATRIX= 32,
	GET_VALUE_AT_TIME_QUAT	= 33,
	ADD_VALUE_PAIR_INT		= 34,
	ADD_VALUE_PAIR_DOUBLE	= 35,
	ADD_VALUE_PAIR_FLOAT	= 36,
	ADD_VALUE_PAIR_BOOL		= 37,
	ADD_VALUE_PAIR_MATRIX	= 38,
	ADD_VALUE_PAIR_QUAT		= 39,
	ADD_VALUE_PROF_INT		= 40,
	ADD_VALUE_PROF_DOUBLE	= 41,
	ADD_VALUE_PROF_FLOAT	= 42,
	ADD_VALUE_PROF_BOOL		= 43,
	ADD_VALUE_PROF_MATRIX	= 44,
	ADD_VALUE_PROF_QUAT		= 45,
	SET_PROF_INT			= 46,
	SET_PROF_DOUBLE			= 47,
	SET_PROF_FLOAT			= 48,
	SET_PROF_BOOL			= 49,
	SET_PROF_MATRIX			= 50,
	SET_PROF_QUAT			= 51
};

/** Which method of Sun stub class is called. */
enum SUN_STUB_FCN {
	GET_SOLAR_CONSTANT		= 1,
	GET_SUN_VEC				= 2,
	CAST_SHADOW				= 3
};

/** Which method of Target stub class is called. */
enum TARGET_STUB_FCN {
	GET_NAME				= 1,
	GET_TARGET_TYPE			= 2,
	GET_VALUE				= 3
};

/** Which method of Task stub class is called. */
enum TASK_STUB_FCN {
	GET_TASK_TYPE			= 1,
	GET_MAX_TIMES			= 2
};

/** Which method of simParams stub class is called. */
enum SIM_PARAMS_STUB_FCN {
	GET_SIM_START_JD		= 1,
	GET_SIM_START_SECONDS	= 2,
	GET_SIM_END_SECONDS		= 3
};

/**
 * A struct to hold information about a stub function call.
 */
struct DATA_HEADER_NS {
	/**
	 * The 
	 */
	BYTE type;
	BYTE stubType; // if type is return, this will hold true / false
	/** The size of the message in bytes. */
	uint32_t size;
};

/**
 * Encodes a DATA_HEADER_NS along with arguments into a buffer with extra space for arguments to be added.
 * Arguments should be copied to the buffer starting at an offset of 2*TYPE_BYTES + SIZE_BYTES.
 * This message will set the size attribute of the header automatically.
 * @param header The DATA_HEADER_NS of the message.
 * @param extra The extra space for arguments that should be created in the buffer for arguments.
 * @param msgSize Will hold the total size of the buffer when the function returns.
 * @return The buffer that is created.
 */
char* encodeDataHeaderNSEx(DATA_HEADER_NS& header, uint32_t extra, uint32_t& msgSize);

/**
 * Decodes a DATA_HEADER_NS from a buffer.
 * @param buf The buffer containing header data.
 * @return The header read from the buffer.
 */
DATA_HEADER_NS decodeDataHeaderNS(char* buf);

}}} // horizon::net::sub

#endif