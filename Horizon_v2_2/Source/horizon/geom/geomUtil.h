#ifndef GEOM_UTIL_H
#define GEOM_UTIL_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <map>

#include "../util/Matrix.h"
#include "../util/umath.h"
#include "../../simParams.h"


namespace horizon {
	namespace geom {

		using horizon::util::Matrix;
		using horizon::util::norm;
		using horizon::util::umath::dot;
		using horizon::util::umath::cross;

/**
 * Returns whether the static or predetermined positions 1 and 2 are accessible from each other
 * @param posECI1 The first position
 * @param posECI2 The second position
 * @return true if the target is accessible from the asset
 */
bool hasLOS(Matrix posECI1, Matrix posECI2);

/**
* Converts the Position of an object from LLA (Longitude, Latitude, Altitude) to 
* ECI (Earth-centered inertial) (XYZ) coordinate system
* @param LLA the Position of the object
* @param JD the Julian Date at which to find the XYZ position
* @return a column vector Matrix with X, Y, and Z position
*/
Matrix LLA2ECI(const Matrix& LLA, const double& JD);

/**
* Converts the Position of an object from ECI (Earth-centered inertial) (XYZ) to 
* LLA (Longitude, Latitude, Altitude) coordinate system
* @param ECI The Position of the object.
* @param JD The Julian Date at which to find the LLA position.
* @return A column vector Matrix in the form of Longitude, Latitude, Altitude.
*/
Matrix ECI2LLA(const Matrix& ECI, const double& JD);

/**
* Converts HMS (Hours, Minutes, Seconds) to UT (Universal Time)
* @param hour the time at which to return the state
* @param minute the time at which to return the state
* @param seconds the time at which to return the state
* @return a Matrix representing the state of the system at the time
*/
double HMS2UT(const unsigned& hour, const unsigned& minute, const double& seconds);

/**
* Converts YMDUT (Year, Month, Day, Universal Time) to JD (Julian Day)
* @param year the year in YMDUT
* @param month the month in YMDUT
* @param day the day in YMDUT
* @param UT the UT in YMDUT
* @return a double representing the Julian Day 
*/
double YMDUT2JD(const unsigned& year, const unsigned& month, const unsigned& day, const double& UT);

/**
 * Convert from Local Civilian Time, (CT) to Local Mean Sidereal Time, (LST)
 * @param longitude the East Longitude of the location where the Local Sidereal Time is desired
 * @param JD the Local Julian Date at the location where the Local Sidereal Time is desired
 * @return the Local Civilian Time in degrees
 */
double CT2LST(const double longitude, const double JD);

}} // end namespace horizon::geom
#endif /* GEOM_UTIL_H_ */