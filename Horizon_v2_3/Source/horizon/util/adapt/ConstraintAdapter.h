/**
 * \file ConstraintAdapter.h
 * This file contains the ConstraintAdapter class.
 * @author Brian Butler
 */

#ifndef CONSTRAINTADAPTER_H
#define CONSTRAINTADAPTER_H

// C++ Includes
#include <iostream>
#include <string>
#include <map>

// HSF Includes
#include "horizon/Constraint.h"
#include "horizon/constraints/maxBatteryDOD.h"
#include "horizon/constraints/maxDataBuffer.h"

// C++ Usings
using namespace std;

// HSF Usings
using namespace horizon::constraints;

namespace horizon {
namespace util {
namespace adapt {

/**
 * This class provides a method for creating Constraints by specifiying the type.
 * @author Brian Butler
 */
class ConstraintAdapter {
	/**
	 * typedef for a wrapper function which calls a default Constraint constructor.
	 */
	typedef Constraint* (ConstraintAdapter::*fcnPtr)();

private:
	// maps to map a string to the correct initialization function
	map<string, fcnPtr> fcnMap;

public:
	/**
	 * A constructor for the ConstraintAdapter class.
	 */
	ConstraintAdapter();

	/**
	 * Maps strings to pointers to functions that will create Constraints.
	 */
	bool initialize();
	
	/**
	 * Clears out the maps used.
	 * @return true if successful.
	 */
	bool finalize();

	/**
	 * Creates a Constraint using the specified key.
	 * The Constraint is created using the default constructor.
	 * @param key The key that the wrapper function for the default
	 * constructor was added to <CODE>fcnMap</CODE> with in the
	 * <CODE>initialize()</CODE> function.
	 * @return The newly created Subsystem.
	 */
	Constraint* create(string key);

private:
	// Wrapper functions for Constraint constructors. These are necessary because
	// function pointers to constructors are not allowed
	Constraint* create_maxBatteryDOD();
	Constraint* create_maxDataBuffer();
};

}}} // horizon::util::adapt

#endif