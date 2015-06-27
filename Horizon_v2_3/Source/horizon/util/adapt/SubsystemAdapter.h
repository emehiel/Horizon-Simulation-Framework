/**
 * \file SubsystemAdapter.h
 * This file contains the SubsystemAdapter class.
 * @author Brian Butler
 */

#ifndef SUBSYSTEMADAPTER_H
#define SUBSYSTEMADAPTER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "horizon/Subsystem.h"
#include "horizon/io/xmlParser.h"

#include "horizon/sub/Access.h"
#include "horizon/sub/Adcs.h"
#include "horizon/sub/Comm.h"
#include "horizon/sub/EOSensor.h"
#include "horizon/sub/Power.h"
#include "horizon/sub/Ssdr.h"

using namespace std;

namespace horizon {
namespace util {
namespace adapt {

/**
 * This class provides a method for creating Subsystems by specifiying the type.
 * Both default constructors and constructors taking an XMLNode as an argument
 * can be used.
 * @author Brian Butler
 */
class SubsystemAdapter {
	// typedefs for function pointers to create the subsystems
	/**
	 * typedef for a wrapper function which calls a default Subsystem constructor.
	 */
	typedef Subsystem* (SubsystemAdapter::*fcnPtr)();
	/**
	 * typedef for a wrapper function which calls a Subsystem constructor that takes an XMLNode as an argument.
	 */
	typedef Subsystem* (SubsystemAdapter::*xmlFcnPtr)(XMLNode);

private:
	// maps to map a string to the correct initialization function
	map<string, fcnPtr> fcnMap;
	map<string, xmlFcnPtr> xmlFcnMap;

public:
	/**
	 * A constructor for the SubsystemAdapter class.
	 */
	SubsystemAdapter();

	/**
	 * Maps strings to pointers to functions that will create Subsystems.
	 * Private member functions must be created first to act as a wrapper for Subsystem constructors.
	 * For example, the following member functions would be needed as wrappers for an Access Subsystem:
	 * <CODE>Subsystem* SubsystemAdapter::create_Access() { return new Access(); }</CODE><BR>
	 * <CODE>Subsystem* SubsystemAdapter::create_Access(XMLNode subNode) { return new Access(subNode); }</CODE><BR>
	 * The first member function acts as a wrapper for the default constructor and takes no arguments. The second
	 * function acts as a wrapper for the constructor taking an XMLNode parameters, so this function takes it as
	 * an argument as well and simply passes it through to the Subsystem constructor.
	 *
	 * The code in <CODE>initialize()</CODE> must be modified to add these wrapper functions.<BR>
	 * <CODE>fcnPtr ptr = &SubsystemAdapter::create_Access;</CODE><BR>
	 * <CODE>xmlFcnPtr ptr2 = &SubsystemAdapter::create_Access;</CODE><BR>
	 * <CODE>fcnMap.insert(make_pair(string("Access"), ptr));</CODE><BR>
	 * <CODE>xmlFcnMap.insert(make_pair(string("Access"), ptr2));</CODE><BR><BR>
	 * 
	 * A default constructed Subsystem can then be created by calling the <CODE>create(string key)</CODE> of the SubsystemAdapter<BR>
	 * <CODE>SubsystemAdapter subAdapter; // creates the SubsystemAdapter</CODE><BR>
	 * <CODE>subAdapter.initialize(); // initializes all mappings</CODE><BR>
	 * <CODE>Subsystem* access = subAdapter.create("Access"); // create the Subsystem using default constructor</CODE><BR>
	 * Note how the same <CODE>key</CODE> that the wrapper function was inserted into <CODE>fcnMap</CODE> in the <CODE>initialize()</CODE>
	 * must be used when creating the Subsystem.<BR>
	 * If the XMLNode constructor is desired to be used instead, then it must be passed to the
	 * overloaded <CODE>create(string key, XMLNode subNode)</CODE> method:
	 * <CODE>Subsystem* access = subAdapter.create("Access", subNode); // create the Subsystem using XMLNode constructor</CODE><BR>
	 *
	 * @return true if successful.
	 * @see create(string key)
	 * @see create(string key, XMLNode subNode)
	 */
	bool initialize();
	
	/**
	 * Clears out the maps used.
	 * @return true if successful.
	 */
	bool finalize();

	/**
	 * Creates a Subsystem using the specified key.
	 * The Subsystem is created using the default constructor.
	 * @param key The key that the wrapper function for the default
	 * constructor was added to <CODE>fcnMap</CODE> with in the
	 * <CODE>initialize()</CODE> function.
	 * @return The newly created Subsystem.
	 * @see initialize()
	 */
	Subsystem* create(string key);

	/**
	 * Creates a Subsystem using the specified key and XMLNode.
	 * The Subsystem is created using the XMLNode constructor.
	 * @param key The key that the wrapper function for the default
	 * constructor was added to <CODE>fcnMap</CODE> with in the
	 * <CODE>initialize()</CODE> function.
	 * @param subNode The XMLNode that is passed to the Subsystem constructor.
	 * @return The newly created Subsystem.
	 * @see initialize()
	 */
	Subsystem* create(string key, XMLNode subNode);

private:
	// Wrapper functions for Subsystem constructors. These are necessary because
	// function pointers to constructors are not allowed
	Subsystem* create_Access();
	Subsystem* create_Access(XMLNode subNode);
	Subsystem* create_Adcs();
	Subsystem* create_Adcs(XMLNode subNode);
	Subsystem* create_Comm();
	Subsystem* create_Comm(XMLNode subNode);
	Subsystem* create_EOSensor();
	Subsystem* create_EOSensor(XMLNode subNode);
	Subsystem* create_Power();
	Subsystem* create_Power(XMLNode subNode);
	Subsystem* create_Ssdr();
	Subsystem* create_Ssdr(XMLNode subNode);
	
};

}}}

#endif
