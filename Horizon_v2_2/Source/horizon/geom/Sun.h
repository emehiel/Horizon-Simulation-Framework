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
		PENUMBRA,
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
	* Generates a Sun object
	*/
	Sun(bool isSunVecConstant);

	/** Default Sun Destructor */
	~Sun(void);

	/** 
	* Return the solar constant of the sun at the earth
	* @return the solar constant in W/m^2
	*/
	const int getSolarConstant() const;

	/**
	* Code from "Fundamentals of Astrodynamics and Applications."
	* Calculates the Earth-Sun vector in [km] according to the simualtion time
	* If sun is created with isEarthSunVecConstant set to true, the value for the vector when the sun is created will be alwyas be used
	* @param simTime the simulation time at which the Earth-Sun vector is required
	* @return a Matrix containing the Earth-Sun vector in ECI
	*/
	const Matrix getEarSunVec(const double simTime) const;

	/**
	* Computes whether the position matrix given is located in the shadow of 
	* the Earth, and it determines which shadow: the Penumbra or Umbra. This
	* function calls the getEarSunVec function to retrieve the Earth-Sun vector. 	
	* Code from "Fundamentals of Astrodynamics and Applications"
	* @param time the simulation time that the shadow determination is requested
	* @returns UMBRA, PENUMBRA, or NO_SHADOW depending on the case
	*/
	shadow_state castShadowOnPos(Position* assetPos, const double& simTime);

	private:
		const int solar_Constant;
		const bool isSunVecConstant;
		Matrix esVec;
};

inline const int Sun::getSolarConstant() const{
		return solar_Constant; }

}} // end namespace horizon::geom
#endif // SUN_H //