/**
* \file SubsystemParameters.h
* This file contains the SubsystemParameters class.
* @author Brian Butler
*/

#ifndef SUBSYSTEM_PARAMETERS_H
#define SUBSYSTEM_PARAMETERS_H

// STL Includes
#include <map>
#include <string>

// HSF Includes
#include "horizon/util/Matrix.h"
#include "horizon/util/Quat.h"

// Standard Library Usings
using namespace std;

// HSF Usings
using horizon::util::Matrix;
using horizon::util::Quat;

namespace horizon {
	namespace sub {
		namespace script {

/** typedef for the map that maps a parameter name to a double parameter value */
typedef map<string, double> DoubleParamMap;
/** typedef for the map that maps a parameter name to a float parameter value */
typedef map<string, float> FloatParamMap;
/** typedef for the map that maps a parameter name to an int parameter value */
typedef map<string, int> IntParamMap;
/** typedef for the map that maps a parameter name to a bool parameter value */
typedef map<string, bool> BoolParamMap;
/** typedef for the map that maps a parameter name to a Matrix parameter value */
typedef map<string, Matrix> MatrixParamMap;
/** typedef for the map that maps a parameter name to a Quat parameter value */
typedef map<string, Quat> QuatParamMap;

/**
 * A class to hold the subsystem parameters for a ScriptedSubsystem.
 * This class is necessary to allow a ScriptedSubsytem to have parameters
 * which it can later look up and use during the simulation.
 * @author Brian Butler
 */
class SubsystemParameters {

private:
	/* These member variables store the subsystem parameters.
		The string is the parameter name and is used as a key
		to access the parameter value */
	DoubleParamMap dParams;
	FloatParamMap fParams;
	IntParamMap iParams;
	BoolParamMap bParams;
	MatrixParamMap mParams;
	QuatParamMap qParams;

public:
	/** The constructor for the SubsystemParameters class. */
	SubsystemParameters();

	/** The destructor for the SubsystemParameters class. */
	~SubsystemParameters();

	/**
	 * Adds a double parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getDoubleParameter(string key)</CODE>.
	 *
	 * @param key The name of the double parameter
	 * @param value The value of the double parameter
	 * @see getDoubleParameter(string key)
	 */
	void addDoubleParameter(string key, double value);

	/**
	 * Adds a float parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getFloatParameter(string key)</CODE>.
	 *
	 * @param key The name of the float parameter
	 * @param value The value of the float parameter
	 * @see getFloatParameter(string key)
	 */
	void addFloatParameter(string key, float value);

	/**
	 * Adds an int parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getIntParameter(string key)</CODE>.
	 *
	 * @param key The name of the int parameter
	 * @param value The value of the int parameter
	 * @see getIntParameter(string key)
	 */
	void addIntParameter(string key, int value);

	/**
	 * Adds a bool parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getBoolParameter(string key)</CODE>.
	 *
	 * @param key The name of the bool parameter
	 * @param value The value of the bool parameter
	 * @see getBoolParameter(string key)
	 */
	void addBoolParameter(string key, bool value);

	/**
	 * Adds a Matrix parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getMatrixParameter(string key)</CODE>.
	 *
	 * @param key The name of the Matrix parameter
	 * @param value The value of the Matrix parameter
	 * @see getMatrixParameter(string key)
	 */
	void addMatrixParameter(string key, Matrix value);

	/**
	 * Adds a Quat parameter.
	 * The parameter can be obtained once it is added
	 * by calling <CODE>getQuatParameter(string key)</CODE>.
	 *
	 * @param key The name of the Quat parameter
	 * @param value The value of the Quat parameter
	 * @see getQuatParameter(string key)
	 */
	void addQuatParameter(string key, Quat value);

	/**
	 * Gets a double parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case double().
	 *
	 * @param key The name of the double parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addDoubleParameter(string key, double value)</CODE>.
	 * @return The value of the double parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>double()</CODE>
	 * is returned.
	 * @see addDoubleParameter(string key, double value)
	 */
	double getDoubleParameter(string key);

	/**
	 * Gets a float parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case float().
	 *
	 * @param key The name of the float parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addFloatParameter(string key, float value)</CODE>.
	 * @return The value of the float parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>float()</CODE>
	 * is returned.
	 * @see addFloatParameter(string key, float value)
	 */
	float getFloatParameter(string key);

	/**
	 * Gets an int parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case int().
	 *
	 * @param key The name of the int parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addIntParameter(string key, int value)</CODE>.
	 * @return The value of the int parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>int()</CODE>
	 * is returned.
	 * @see addIntParameter(string key, int value)
	 */
	int getIntParameter(string key);

	/**
	 * Gets a bool parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case bool().
	 *
	 * @param key The name of the bool parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addBoolParameter(string key, bool value)</CODE>.
	 * @return The value of the bool parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>bool()</CODE>
	 * is returned.
	 * @see addBoolParameter(string key, bool value)
	 */
	bool getBoolParameter(string key);

	/**
	 * Gets a Matrix parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case Matrix().
	 *
	 * @param key The name of the Matrix parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addMatrixParameter(string key, Matrix value)</CODE>.
	 * @return The value of the Matrix parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>Matrix()</CODE>
	 * is returned.
	 * @see addMatrixParameter(string key, Matrix value)
	 */
	Matrix getMatrixParameter(string key);

	/**
	 * Gets a Quat parameter corresponding to the specified key.
	 * If the key does not exist in the map, the default return type
	 * value is returned, in this case Quat().
	 *
	 * @param key The name of the Quat parameter to retrieve.
	 * This must be the same key that the parameter was added with
	 * using <CODE>addQuatParameter(string key, Quat value)</CODE>.
	 * @return The value of the Quat parameter. If no parameter
	 * was found with <CODE>key</CODE>, then <CODE>Quat()</CODE>
	 * is returned.
	 * @see addQuatParameter(string key, Quat value)
	 */
	Quat getQuatParameter(string key);

};

// Inline function definitions
inline void SubsystemParameters::addDoubleParameter(string key, double value) {
	// Insert parameter into map
	dParams.insert(make_pair(key, value));
}

inline void SubsystemParameters::addFloatParameter(string key, float value) {
	// Insert parameter into map
	fParams.insert(make_pair(key, value));
}

inline void SubsystemParameters::addIntParameter(string key, int value) {
	// Insert parameter into map
	iParams.insert(make_pair(key, value));
}

inline void SubsystemParameters::addBoolParameter(string key, bool value) {
	// Insert parameter into map
	bParams.insert(make_pair(key, value));
}

inline void SubsystemParameters::addMatrixParameter(string key, Matrix value) {
	// Insert parameter into map
	mParams.insert(make_pair(key, value));
}

inline void SubsystemParameters::addQuatParameter(string key, Quat value) {
	// Insert parameter into map
	qParams.insert(make_pair(key, value));
}

}}} // horizon::sub::Script

#endif