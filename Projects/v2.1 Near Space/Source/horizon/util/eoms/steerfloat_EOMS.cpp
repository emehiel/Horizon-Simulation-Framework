#include "steerfloat_EOMS.h"

namespace horizon{
namespace util{
namespace eoms{

ss_EOMS::ss_EOMS(horizon::util::Matrix &A):EOMS(),g(-9.81) {}

ss_EOMS::ss_EOMS(): EOMS(), g(-9.81)
{}

ss_EOMS::~ss_EOMS(){}
						
Matrix ss_EOMS::operator () (const double &t,const Matrix &y)
{
					
	double Density = y.getValue(7,2);
	double ZonalWind = y.getValue(3,1);
	double Zonal_sail_wind = y.getValue(4,1);
	double frontarea = y.getValue(5,1);
	double ZonalInitVel = y.getValue(2,1);
	double MeridionalWind = y.getValue(3,2);
	double Meridonal_sail_wind = y.getValue(4,2);
	double MeridionalInitVel = y.getValue(2,2);
	double mPayload = y.getValue(6,1);
	double ssGasMass = y.getValue(7,3);
	double volBalloon = y.getValue(5,2);
	double AltInitVel = y.getValue(2,3);				
	double Bal_Lat = y.getValue(1,1);
	double Bal_Long = y.getValue(1,2);
	double Bal_Alt = y.getValue(1,3);
	double radius = y.getValue(5,3);
	double balloonMass = y.getValue(6,3);
	double visc = y.getValue(7,1);
	double JD = y.getValue(3,3);
	double alpha = y.getValue(4,3);
	double densityAtSail = y.getValue(8,1);
	double SSA = y.getValue(8,1);
	double sailMass = y.getValue(6,2);
	double AR = y.getValue(8,3); //Aspect Ratio of sail
	double tao = y.getValue(9,1); //tao is function of forier coefficients and needs to be set in initial conditions
							      //ranges from 0.05 & 0.25 (fund. of aerodynamics Anderson pg. 380
	double Cd=y.getValue(9,3);	  //zero lift coefficient
	double e=y.getValue(9,2);     //efficientcy factor of airfoil

	double massOfBalloon = ssGasMass+balloonMass;
	double rho0 = ssGasMass/volBalloon;
	double relativeBallMass = (1-(Density/rho0))*(massOfBalloon);
	double ssmsys = mPayload+massOfBalloon+sailMass;
	double topArea = frontarea;

	double ZonalVel;
	double MeridionalVel;
	double Zonal_Cd;
	double Meridional_Cd;
	double Top_Cd;

	double VZ = ZonalWind-ZonalInitVel;
	double VM = MeridionalWind-MeridionalInitVel;
	
	double Zonal_Re = abs(((radius*2)*ZonalWind)/visc);
	double Meridional_Re = abs(((radius*2)*MeridionalWind)/visc);
	double Top_Re = abs(((radius*2)*AltInitVel)/visc);

	//Approximations are taken from equation (3-225) in Viscous fluid flow Frank M. White
	//balloon is assumed smooth sphere

	if (Zonal_Re>=0 && Zonal_Re<=250000) //from 100000-250000 accuracy is +- 10%
		Zonal_Cd = (24/Zonal_Re)+(6/(1+sqrt(Zonal_Re))+0.4);
	else if (Zonal_Re>250000) //flow is turbulent at this region
		Zonal_Cd = .2; //This is temporary...need a calcuation or set of data for turbulent flow
	else
		cout<<"error Reynolds number cannot be below 0"<<endl;
	exit;

	if (Meridional_Re>=0 && Meridional_Re<=250000)				//from 100000-250000 accuracy is +- 10%
		Meridional_Cd = (24/Meridional_Re)+(6/(1+sqrt(Meridional_Re))+0.4);
	else if (Meridional_Re>250000)					//flow is turbulent at this region
		Meridional_Cd = .2; 
	else
		cout<<"error Reynolds number cannot be below 0"<<endl;
	exit;

	if (Top_Re>0 && Top_Re<=250000)                          //from 100000-250000 accuracy is +- 10%
		Top_Cd = (24/Top_Re)+(6/(1+sqrt(Top_Re))+0.4);
	else if (Top_Re>250000)                                     //flow is turbulent at this region
		Top_Cd = .2;    
	else if (Top_Re == 0)
		Top_Cd = 0;
	else														  
		cout<<"error Reynolds number cannot be below 0"<<endl;
	exit;


	
	double F_Zonal = .5*Density*abs(VZ)*frontarea*Zonal_Cd*VZ;
	double F_Meridional = .5*Density*abs(VM)*frontarea*Meridional_Cd*VM;
	double Fl = (relativeBallMass-(mPayload+sailMass))*g;
	double Fd = (.5*Density*abs(AltInitVel)*topArea*Top_Cd*AltInitVel);
	double Fz;
	if (abs(Fd)>abs(Fl))
		Fz = 0;
	else
		Fz = Fl-Fd;
	
	ZonalVel = ZonalInitVel+(F_Zonal/ssmsys)*t;
	MeridionalVel = MeridionalInitVel+(F_Meridional/ssmsys)*t;
	double AltVel = AltInitVel+(Fz/ssmsys)*t;

	if (abs(ZonalVel)>abs(ZonalWind))
		ZonalVel = ZonalWind;
	if (abs(MeridionalVel)>abs(MeridionalWind))
		MeridionalVel = MeridionalWind;
   

	double Zonal_Relative_V = ZonalVel+Zonal_sail_wind;
	double Meridional_Relative_V = MeridionalVel+Meridonal_sail_wind;

	double a = (2*M_PI)/(1+((2*M_PI)/(M_PI*AR))*(1+tao));
	double CL = a*alpha;  
	double CD = Cd+((CL*CL)/(M_PI*e*AR)); 

	double L_Zonal = CL*.5*densityAtSail*Zonal_Relative_V*SSA;
	double L_Meridional = CL*.5*densityAtSail*Meridional_Relative_V*SSA;

	double D_Zonal = CD*.5*densityAtSail*Zonal_Relative_V*SSA;
	double D_Meridional = CD*.5*densityAtSail*Meridional_Relative_V*SSA;

	double F_Zonalsail = L_Zonal-D_Zonal;
	double F_Meridionalsail = L_Meridional-D_Meridional;
	
	double MeridionalsailVel;
	double ZonalsailVel;

	ZonalsailVel = (F_Zonalsail/ssmsys)*t;
	MeridionalsailVel = (F_Meridionalsail/ssmsys)*t;

	double newZonalVel = ZonalVel+ZonalsailVel;
	double newMeridionalVel = MeridionalVel+MeridionalsailVel;
	
	double Zonal_Disp = (.5*ZonalVel*t)/1000;		
	double Meridional_Disp = (.5*MeridionalVel*t)/1000;
	double Alt_Disp = .5*AltVel*t;


	if (abs(AltVel)>20)
		cout<<"oops"<<endl;

	if (abs(ZonalVel)>50)
		cout<<"oops"<<endl;

	if (abs(MeridionalVel)>50)
		cout<<"oops"<<endl;


	Matrix LLA(3,1,0.0);
	LLA.setValue(1,1,Bal_Lat);
	LLA.setValue(2,1,Bal_Long);
	LLA.setValue(3,1,Bal_Alt);
	Matrix Bal_ECI(1,3,0.0);
	Bal_ECI = LLA2ECI(LLA,JD);
	double X = Bal_ECI.getValue(1,1);
	double Y = Bal_ECI.getValue(2,1);
	double Z = Bal_ECI.getValue(3,1);
	double recalcx = X+Zonal_Disp;
	double recalcy = Y+Meridional_Disp;
	double recalcz = Z+Alt_Disp;
	Matrix ECI(3,1,0.0);
	ECI.setValue(1,1,recalcx);
	ECI.setValue(2,1,recalcy);
	ECI.setValue(3,1,recalcz);
	Matrix Bal_LLA = ECI2LLA(ECI,JD);
	double newLat = Bal_LLA.getValue(1,1);
	double newLong = Bal_LLA.getValue(1,2);
	double newAlt = Bal_LLA.getValue(1,3);
	Matrix current_state(6,3,0.0);				
	current_state.setValue(1,1,newLat);
	current_state.setValue(2,1,newZonalVel);
	current_state.setValue(3,1,ZonalWind);
	current_state.setValue(1,2,newLong);
	current_state.setValue(2,2,newMeridionalVel);
	current_state.setValue(2,3,MeridionalWind);
	current_state.setValue(1,3,newAlt);
	current_state.setValue(2,3,AltVel);
	current_state.setValue(6,2,Density);
	current_state.setValue(4,1,frontarea);
	current_state.setValue(5,1,mPayload);
	current_state.setValue(4,2,volBalloon);
	current_state.setValue(5,3,ssGasMass);
	current_state.setValue(4,3,radius);
	current_state.setValue(5,2,sailMass);
	current_state.setValue(6,3,balloonMass);
	return current_state;
};
}
}
}
			