/**
 * \file Environment.h
 * This file contains the Environment class.
 * @author Brian Butler
 * @author Eric Mehiel
 * @author Travis Lockyer
 * @author Seth Silva
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Sun.h"
#include "horizon/io/xmlParser.h"
#include "horizon/util/util.h"

using namespace horizon::io;
using namespace horizon::util;

namespace horizon {
	namespace geom {

/**
 * A model of the Environment the System resides in.
 * @author Brian Butler
 * @author Eric Mehiel
 * @author Travis Lockyer
 * @author Seth Silva
 */
class Environment {

public:
	/**
	* The constructor for the Environment class.
	*/
	Environment();

	/**
	* The constructor for the Environment class.
	* Ths constructor creates an Environment with the specified XMLNode.
	* @param environmentNode The XMLNode with the information to create the environment.
	*/
	Environment(XMLNode& environmentNode);

	/**
	* The constructor for the Environment class.
	* This constructor creates an with Environment with the specified Sun.
	* @param sun A pointer to a Sun that this Environment will use.
	*/
	Environment(Sun* sun);

	/**
	 * The destructor for the Environment class.
	 * The destructor will delete the Sun object that is held.
	 */
	~Environment();

	/**
	 * Gets the Sun for the Environment.
	 * @return A pointer to the Sun object.
	 */
	inline Sun* Environment::getSun(){
		return sun;
	}

private:
	Sun* sun;
};

}} // end namespace horizon::geom
#endif /* ENVIRONMENT_H_ */