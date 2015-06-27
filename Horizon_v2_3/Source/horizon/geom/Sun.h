/**
 * \file Sun.h
 * This file contains the Sun class.
 * @author Brian Butler
 * @author Seth Silva
 */

#ifndef SUN_H
#define SUN_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <map>
#include <string>
#include "../util/Matrix.h"
#include "Position.h"
#include "../../simParams.h"

namespace horizon {
namespace geom {

/**
* The state of shadow caused by the earth on a position*
*/
enum shadow_state {
	NO_SHADOW,
	UMBRA,
	PENUMBRA
};

using std::cout;
using std::map;
using std::string;
using namespace horizon::util::umath;

/**
* Creates a Sun model able to compute the Sun Vector to the Earth as well as 
* determining whether the spacecraft is in the shadow of the Earth.
* @author Brian Butler
* @author Seth Silva
*/

class Sun
{
public:
	/**
	* The constructor for the Sun class.
	*/
	Sun();

	/**
	* The constructor for the Sun class.
	* @param isSunVecConstant A flag that determines whether to hold
	* the Sun vector constant. If set to true, future calls to getEarSunVec(const double simTime)
	* will always return the value at a <CODE>simTime</CODE> of 0 seconds.
	*/
	Sun(bool isSunVecConstant);

	/** The destructor for the Sun class. */
	~Sun(void);

	/** 
	* Returns the solar constant of the Sun at the earth.
	* @return The solar constant in W/m^2.
	*/
	const int getSolarConstant() const;

	/**
	* Calculates the Earth-Sun vector in [km] according to the simualtion time.
	* If the Sun is created with isEarthSunVecConstant set to true, the value for the
	* vector at a simulation time of 0 seconds will be alwyas be used.
	* Code from "Fundamentals of Astrodynamics and Applications."
	* @param simTime the simulation time at which the Earth-Sun vector is required
	* @return a Matrix containing the Earth-Sun vector in ECI.
	*/
	const Matrix getEarSunVec(const double simTime) const;

	/**
	* Casts a shadow on the specified Position.
	* Computes whether the position matrix given is located in the shadow of 
	* the Earth, and it determines which shadow: the Penumbra or Umbra. This
	* function calls the getEarSunVec function to retrieve the Earth-Sun vector. 	
	* Code from "Fundamentals of Astrodynamics and Applications"
	* @param time the simulation time that the shadow determination is requested
	* @returns UMBRA, PENUMBRA, or NO_SHADOW depending on the case.
	*/
	shadow_state castShadowOnPos(Position* assetPos, const double& simTime);

	private:
		const int solar_Constant;
		const bool isSunVecConstant;
		Matrix esVec;
};

inline const int Sun::getSolarConstant() const {
		return solar_Constant;
}

}} // end namespace horizon::geom
#endif // SUN_H //