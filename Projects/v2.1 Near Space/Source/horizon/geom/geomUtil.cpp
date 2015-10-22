#include "geomUtil.h"

namespace horizon {
	namespace geom {


bool hasLOS(Matrix posECI1, Matrix posECI2){
	// calculate the minimum distance to the center of the earth
	double d = norm(cross(posECI2 - posECI1, posECI1))/norm(posECI2 - posECI1);
	/* parameter t is is the parameter that represents where the minimum distance is
	d is a minimum at (posECI1) + t*(posECI2 - posECI1)*/
	double t = -dot(posECI1, posECI2 - posECI1)/pow(norm(posECI2 - posECI1),2);
	/* if t > 1 or t < 0, then the minumim distance does not occur along the line segment connecting positions 1 and 2,
	and the two positions are visible from each other*/
	if(t >= 1 || t <= 0)
		return true;
	// otherwise compare the minimum distance to the radius of the earth
	return d >= EARTH_RADIUS;
}

Matrix LLA2ECI( const Matrix& LLA, const double& JD )
{
	Matrix pos(3,1,0.0);

	double lat = LLA.getValue(1,1); //deg
	double lon = LLA.getValue(2,1); //deg
	double alt = LLA.getValue(3,1); //km

	double gst = CT2LST(lon, JD); //deg
	double theta = gst * M_PI/180.0; //(lon + gst) * M_PI/180.0; //rad

	double r = (EARTH_RADIUS + alt)*cos(lat*M_PI/180.0); // km
	
	pos.setValue(1,1, r*cos(theta));// km
	pos.setValue(2,1, r*sin(theta));// km
	pos.setValue(3,1, (alt+EARTH_RADIUS)*sin(lat*M_PI/180.0));// km

	return pos;
}

Matrix ECI2LLA( const Matrix& ECI, const double& JD )
{
	Matrix pos(3,1,0.0);
	double x = ECI.getValue(1,1);
	double y = ECI.getValue(2,1);
	double z = ECI.getValue(3,1);
	
	double r = norm(ECI);
	double templon = 180/M_PI*atan2(y, x); //deg
	double diff = templon - CT2LST(templon, JD);
	double lon = templon + diff;

	pos.setValue(1,1, 180/M_PI*atan2(z, sqrt(pow(x,2)+pow(y,2)))); //deg
	pos.setValue(2,1, lon); //deg
	pos.setValue(3,1, r-EARTH_RADIUS); //km
	return pos;
}

double HMS2UT( const unsigned& h, const unsigned& m, const double& seconds)
{
	return double(h) + double(m)/60.0 + seconds/3600.0;
}

double YMDUT2JD( const unsigned& y, const unsigned& m, const unsigned& d, const double& UT)
{
	return 367*y - floor(7.0*(y + floor((m + 9.0)/12.0))/4.0) + floor(275.0*m/9.0) + d + 1721013.5 + UT/24.0;
}

double CT2LST( const double longitude, const double JD )
{
	double J0 = floor(JD + 0.5) - 0.5;
	double UT = (JD - J0)*24.0;
	double T0 = (J0 - 2451545.0)/36525.0;
	double c[5] = {100.4606184, 36000.77004, 0.000387933, -2.583e-8, 360.98564724};
	double g0 = c[0] + c[1]*T0 + c[2]*pow(T0,2) + c[3]*pow(T0,3);
	
	g0 = g0 - 360.0*floor(g0/360.0);

	double gst = g0 + c[4]*UT/24.0;
	double lst = gst + longitude;

	lst = lst - 360.0*floor(lst/360.0);

	return lst;
}

	}} // end namespace horizon::geom