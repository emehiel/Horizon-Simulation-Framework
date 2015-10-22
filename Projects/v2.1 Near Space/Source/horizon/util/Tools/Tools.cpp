#include "Tools.h"


//===========================================================================================================================
	const pair<Windata,Atmosdata> ExactValues(const double alt,map<double,Windata>* windmap, map<double,Atmosdata>* atmosmap) 
	{
	map<double,Windata>::iterator itup;
	map<double,Atmosdata>::iterator itup2;

	//Intepolates the Wind data
	itup = windmap->upper_bound(alt);
				// Here, the iterator below and above the requested key will be
				// found so interpolation can be used to find the approx. value.
	if(itup == windmap->end() )
	{
		//cout<<"Warning the vehicle has exceeded it's data...the calculated values may become inaccurate"<<endl;
		itup--;
	}

	if(itup == windmap->begin())
	{
		//cout<<"Warning...initial values below wind data set...using closest wind speed data"<<endl;
		itup++;
	}
	double altHigh = itup->first;
	Windata windhigh = itup->second;
	double zonehigh = windhigh.writezonal_component();
	double meridhigh = windhigh.writemeridional_component();
	itup--;
	double altLow = itup->first;
	Windata windlow	= itup->second;
	double zonelow = windlow.writezonal_component();
	double meridlow = windlow.writemeridional_component();
	// Iterpolation follows:
	double ExactZone = zonelow+((alt-altLow)/(altHigh-altLow))*(zonehigh-zonelow);
	double ExactMerid = meridlow+((alt-altLow)/(altHigh-altLow))*(meridhigh-meridlow);
	Windata exactwin;
	exactwin.setvalues(ExactZone,ExactMerid);

	//Interpolates the Atmospheric data
	
	itup2 = atmosmap->upper_bound(alt);
	double althigh = itup2->first;
	Atmosdata atmoshigh = itup2->second;
	double Denshigh = atmoshigh.writedensity();
	double Preshigh = atmoshigh.writepressure();
	double Temphigh = atmoshigh.writetemp();
	double vischigh = atmoshigh.writevis();
	itup2--;
	Atmosdata atmoslow = itup2->second;
	double Preslow = atmoslow.writepressure();
	double Denslow = atmoslow.writedensity();
	double Templow = atmoslow.writetemp();
	double visclow = atmoslow.writevis();
	double ExactPress = Preslow+((alt-altLow)/(altHigh-altLow))*(Preshigh-Preslow);
	double ExactDens = Denslow+((alt-altLow)/(altHigh-altLow))*(Denshigh-Denslow);
	double ExactTemp = Templow+((alt-altLow)/(altHigh-altLow))*(Temphigh-Templow);
	double ExactVisc = visclow+((alt-altLow)/(altHigh-altLow))*(vischigh-visclow);

	Atmosdata exactAtmos;
	exactAtmos.setvalues(ExactPress,ExactDens,ExactTemp,ExactVisc);
	const pair<Windata,Atmosdata> p1 (exactwin,exactAtmos);
	return p1;
	};
//=======================================================================================================================
Matrix StationLocation(double lat, double lon, double alt)
{
	Matrix statpos(3,1,0.0);
	statpos.setValue(1,1,lat);
	statpos.setValue(2,1,lon);
	statpos.setValue(3,1,alt);
	return statpos;
};

//======================================================================================================================================================
 pair<double,double> BallGeom(Matrix oldAttribs, pair<Windata,Atmosdata> exact,double GasMass)
{


	double T1 = oldAttribs.getValue(1,1);
	double P1 = oldAttribs.getValue(2,1);
	double V1 = oldAttribs.getValue(3,1);

	Atmosdata atmos = exact.second;
	double T2 = atmos.writetemp();
	double P2 = atmos.writepressure();

	double R1 = pow(((V1/M_PI)*(.75)),0.33333333333333333333333333333333);
	double R = R1*pow(((T2/T1)*(P1/P2)),0.33333333333333333333333333333333); 
	double volBalloon = M_PI*R*R*R*1.333333333333;
	double diam = 2*R;
	double fntArea = (M_PI*diam*diam)/4;
	pair<double, double> ballgeom(fntArea,volBalloon);
	return ballgeom;
	//fntArea =  Surface area of Balloon being hit by the wind.  The size of the payload 
	//is very small compaired to the balloon so it is neglected in these calculations.

 };
//=========================================================================================================================
pair<double,double> ssGeom(Matrix oldAttribs, pair<Windata,Atmosdata>exact,double ssGasMass)
{

	double T1 = oldAttribs.getValue(1,1);
	double P1 = oldAttribs.getValue(2,1);
	double V1 = oldAttribs.getValue(3,1);

	Atmosdata atmos = exact.second;
	double T2 = atmos.writetemp();
	double P2 = atmos.writepressure();


	double volBalloon = (T1*P1*V1)/(T2*P2);

	/*
	Windata windData = exact.first;
	Atmosdata atmosData = exact.second;

	double Density = atmosData.writedensity();

	double volBalloon = ssGasMass/Density;
	*/
	double a = pow(1.28*volBalloon,0.333333333);
	double c = a/1.71;
	double ae = acos(c/a);
	double SA = 2*M_PI*(a*a+c*c*((atan(sin(ae)))/(sin(ae))));
	double fntArea = .5*SA;


	//surface area approximation for oblate sphereoid with equitorial radius
	pair<double,double> ssGeom(fntArea,volBalloon);	return ssGeom;
}
//=========================================================================================================================
 double GeopotheightToGeometAlt(double geoPotHeight,double Lat)
 {	
	 double rpd = 1.74532925199433*pow ((double)10,-2);
	 double Cos2L = cos(2*Lat*rpd);
	 double Cos4L = cos(4*rpd*Lat);
	 double fgnSMT = 9.80616*(1-0.0026373*Cos2L+0.0000059*Cos2L*Cos2L);
	 double fdgdzSMT = -(3.085462 *pow ((double)10,-6) + 2.27 *pow((double)10,-9) * Cos2L - 2 *pow((double)10,-12) * Cos4L);
	 double r = -2 * fgnSMT / fdgdzSMT / 1000;
	 double fZhtoZgSMT = r * geoPotHeight / fgnSMT * r / 9.80665 - geoPotHeight;
	 return fZhtoZgSMT;
 }
 //========================================================================================================================
 double GeometAltToGeopotheight(double GeomAlt)
 {
	 /*
 	 double rpd = 1.74532925199433*pow((double)10,-2);
	 double Cos2L = cos(2*Lat*rpd);
	 double Cos4L = cos(4*rpd*Lat);
	 double fgnSMT = 9.80616*(1-0.0026373*Cos2L+0.0000059*Cos2L*Cos2L);
	 double fdgdzSMT = -(3.085462 *pow((double)10,-6) + 2.27 *pow((double)10,-9) * Cos2L - 2 *pow((double)10,-12) * Cos4L);
	 double r = -2 * fgnSMT / fdgdzSMT / 1000;
	 double fZgToZghSMT = r * (fgnSMT / 9.80665) * (GeomAlt / (GeomAlt + r));
	 return fZgToZghSMT;
	 */
	 double REARTH=6369.0;    // radius of the Earth (km)
	 double altKm = GeomAlt/1000;
	 double altMeters = (altKm*REARTH/(altKm+REARTH))*1000;
	 return altMeters;
 }
 //========================================================================================================================
