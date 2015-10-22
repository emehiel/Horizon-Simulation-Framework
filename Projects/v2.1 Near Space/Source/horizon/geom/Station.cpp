#include "Station.h"

namespace horizon{
	namespace geom{



void Station::setvalues(Matrix Position, int quadrant,string name, map<double,Windata> windmap, map<double,Atmosdata> atmosmap)
{
		Pos = Position;
		Quadrantnum = quadrant;
		Station_Name = name;
		Windmap = windmap;
		Atmosmap = atmosmap;
}

Matrix Station::writePos()
	{
		return Pos;
	}
	
int Station::writeQuadrant()
	{
		return Quadrantnum;
	}

string Station::writename()
	{
		return Station_Name;
		
	}
map <double,Atmosdata>* Station::writeAtmosData()
	{
		return &Atmosmap;
	}

map <double,Windata>* Station::writeWindData()
	{
		return &Windmap;
	}

list<double> Station::writeDateList()
{
	return Datelist;
}
//================================================================================================
map<double,Atmosdata> Station::LoadAtmosData()
	{
	map<double,Atmosdata>atmosDataMap;

//======================================================================================================================

	// P H Y S I C A L   C O N S T A N T S
	const double FT2METERS      = 0.3048;     // mult. ft. to get meters (exact)
	const double KELVIN2RANKINE = 1.8;              // mult kelvins to get deg R
	const double PSF2NSM        = 47.880258;      // mult lb/sq.ft to get N/sq.m
	const double SCF2KCM        = 515.379;    // mult slugs/cu.ft to get kg/cu.m
	const double TZERO          = 288.15;      // sea level temperature, kelvins
	const double PZERO          = 101325.0;        // sea-level pressure, N/sq.m
	const double RHOZERO        = 1.225;           // sea level density, kg/cu.m
	const double AZERO          = 340.294;    // sea-level speed of sound, m/sec

	double altKm; //Altitude is in Kilometers
	double sigma,delta,theta;
	double temp,pressure,density,asound;
	double viscosity,kinematicViscosity;


	
	for(int ind=-1; ind<=43; ind++)
	{
		altKm=2*ind;

		// Compute the temperature,density, and pressure in the standard atmosphere
		// Correct to 86 km.  Only approximate thereafter.

		const double REARTH=6369.0;    // radius of the Earth (km)
		const double GMR = 34.163195;
		const int NTAB = 8;
		int i,j,k;

		static double htab[NTAB] = {0.0,  11.0, 20.0, 32.0, 47.0,
					 51.0, 71.0, 84.852 };
		static double ttab[NTAB] = { 288.15, 216.65, 216.65, 228.65, 270.65,
					  270.65, 214.65, 186.946 };
		static double ptab[NTAB] = { 1.0, 2.2336110E-1, 5.4032950E-2, 8.5666784E-3,
		 1.0945601E-3, 6.6063531E-4, 3.9046834E-5, 3.68501E-6 };
		static double gtab[NTAB] = { -6.5, 0, 1.0, 2.8, 0, -2.8, -2.0, 0 };

		double h=altKm*REARTH/(altKm+REARTH);     //  geometric to geopotential altitude
		double hMeters = h*1000;
	//	double GeomAlt = altKm*1000;
	//	double h=GeometAltToGeopotheight(GeomAlt,lat); // h is geopotential alt in meters
		i=0; j=NTAB-1;  // starting values for binary search
		do
		{
		  k=(i+j)/2;
		  if (h < htab[k]) j=k; else i=k;
		}  while (j > i+1);

		double tgrad=gtab[i];                      // temp. gradient of local layer
		double tbase=ttab[i];                      // base temp. of local layer
		double deltah=h-htab[i];                   // height above local base
		double tlocal=tbase+tgrad*deltah;          // local temperature

		theta=tlocal/ttab[0];                                  // temperature ratio

		if (0.0 == tgrad)                                         // pressure ratio
		delta=ptab[i]*exp(-GMR*deltah/tbase);
		else
		delta=ptab[i]*pow(tbase/tlocal, GMR/tgrad);

		sigma=delta/theta;                                        //  density ratio

		temp = TZERO*theta;
		pressure = PZERO*delta;
		density = RHOZERO*sigma;
		asound = AZERO*sqrt(theta);
		

		const double TZERO = 288.15;               // sea level temperature, kelvins
		const double BETAVISC = 1.458E-6;     // constant, N-sec/(sq.m-sqrt(kelvins)
		const double SUTH = 110.4;                 // Sutherland's constant, kelvins

		double t=theta*TZERO;                              // temperature in kelvins
		viscosity = BETAVISC*sqrt(t*t*t)/(t+SUTH);            // viscosity in kg/(m-sec)
		kinematicViscosity=viscosity/density;


		Atmosdata atmosdata;
		atmosdata.setvalues(pressure,density,t,kinematicViscosity);
		atmosDataMap.insert(pair<double,Atmosdata>(hMeters,atmosdata));
	};
	return atmosDataMap;
}
//===============================================================================================================
}
}