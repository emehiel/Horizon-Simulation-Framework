#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Sun.h"

namespace horizon {
	namespace geom {

/**
 * A model of the Environment the system is in
 * @author Eric Mehiel
 * @author Travis Lockyer
 * @author Seth Silva
 */
class Environment {

public:
	/**
	* Create an Environment
	*/
	Environment();

	/**
	* Create an with Environment with the specified Sun
	* @param sun pointer to a Sun
	*/
	Environment(Sun* sun);

	~Environment();

	inline Sun* Environment::getSun(){
		return sun; }

private:
	Sun* sun;
};

}} // end namespace horizon::geom
#endif /* ENVIRONMENT_H_ */