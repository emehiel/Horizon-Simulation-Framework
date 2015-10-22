#include "Sun.h"

namespace horizon {
namespace geom {

Sun::Sun(bool isSunVecConstant) : solar_Constant(1366), isSunVecConstant(isSunVecConstant)
{
	if(isSunVecConstant)
		esVec = getEarSunVec(0.0);
}

Sun::~Sun() {}

shadow_state Sun::castShadowOnPos(Position* pos, const double& simTime)
{
	double penVert;
	double satHoriz;
	double satVert;
	double sigma;
	double umbVert;
	shadow_state shadow;

	const double alphaPen = 0.26900424;
	const double alphaUmb = 0.26411888;
	const double rad	  = M_PI/180;
	const double rEar	  = 6378.137;

	// Get earth-sun vector
	Matrix rSun = getEarSunVec(simTime);
	// Get the vector from the earth to the object
	Matrix assetPosAtTime = pos->getPosECI(simTime);
	double dot_p = dot((-rSun),assetPosAtTime);
	// Calculate the cosine of the angle between the position vector
	// and the axis the earth-sun vector lies on
	double arg = (dot_p)/(norm(-rSun)*norm(assetPosAtTime));
	//fix argument, must be between -1 and 1
	if(abs(arg) > 1)
		arg = arg/abs(arg)*floor(abs(arg));
	sigma = acos(arg);
	// Calculate the distance from the 
	satHoriz = norm(assetPosAtTime)*cos(sigma);
	satVert  = norm(assetPosAtTime)*sin(sigma);

	// Calculate distance away from earth-sun axis where penumbra ends
	penVert = rEar + tan(alphaPen*rad)*satHoriz;

	// determine the shadow state of the position
	if (dot_p > 0 && satVert <= penVert)
	{
		shadow = PENUMBRA;
		//Calculate distance away from earth-sun axis where umbra ends
		umbVert = rEar - tan(alphaUmb*rad)*satHoriz;

		if (satVert <= umbVert)
			shadow = UMBRA;
	}
	else
		shadow = NO_SHADOW;

	return(shadow);
}

const Matrix Sun::getEarSunVec(const double simTime) const
{
	if(isSunVecConstant && esVec != Matrix())
		return esVec;
	
	double eclLong;
	double meanLongSun;
	double MASun;
	double obl;
	double rSun;
	Matrix RSun(3, 1, 0.0);
	double TUt1;
	double TTdb;
	double JDUt1 = (simTime/86400) + simParams::SIMSTART_JD();

	const double aU	 = 149597870.0;
	const double rad = M_PI/180;
	

	// Computing the number of Julian centuries from the epoch:  
	TUt1 = (JDUt1 - 2451545.0)/ 36525;
	
	// Computing the Mean longitude of the Sun:  
	meanLongSun = 280.460 + 36000.77*TUt1;
	
	// Put into range of 0 to 360 degrees
	if (meanLongSun < 0)
	{
		double meanLongSunDiv = floor(-1*meanLongSun/360);
		meanLongSun = meanLongSun + (meanLongSunDiv+1)*360;
	}
	else if (meanLongSun > 360)
	{
		double meanLongSunDiv = floor(meanLongSun/360);
		meanLongSun = meanLongSun - meanLongSunDiv*360;
	}
	//end if //

	// Juliamn centuries of Barycentric dynamical time are assumed to be equal
	// to the number of Julian centuries from the epoch:  
	TTdb = TUt1;
	
	// Computing the Mean Anomaly of the sun: 
	MASun = 357.5277233 + 35999.05034*TTdb;
	
	// Put into range of 0 to 360 degrees
	if (MASun < 0)
	{
		double MASunDiv = floor(-1*MASun/360);
		MASun = MASun + (MASunDiv+1)*360;
	}
	else if (MASun > 360)
	{
		double MASunDiv = floor(MASun/360);
		MASun = MASun - MASunDiv*360;
	}

	// Computing the ecliptic longitude:  
	eclLong = meanLongSun + 1.914666471*sin(MASun*rad) + 0.019994643*sin(2*MASun*rad);
	
	// Computing the sun-centered position vector from the Sun to Earth:  
	rSun = 1.000140612 - 0.016708617*cos(MASun*rad) - 0.000139589*cos(2*MASun*rad);
	
	// Computing the obliquity of the ecliptic:  
	obl = 23.439291 - 0.0130042*TTdb;

	// Transforming sun_centered Earth position vector to a geocentric
	// equatorial position vector:
	RSun.setValue(1,1,rSun*cos(eclLong*rad)*aU);
	RSun.setValue(2,1,rSun*cos(obl*rad)*sin(eclLong*rad)*aU);
	RSun.setValue(3,1,rSun*sin(obl*rad)*sin(eclLong*rad)*aU);
	
	//if(isSunVecConstant)
	//	esVec = RSun;
	
	return(RSun);
}
}}// end namespace horizon::geom