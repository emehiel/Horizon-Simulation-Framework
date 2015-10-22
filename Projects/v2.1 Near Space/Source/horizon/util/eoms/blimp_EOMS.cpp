#include "blimp_EOMS.h"

namespace horizon{
namespace util{
namespace eoms{

blimp_EOMS::blimp_EOMS(horizon::util::Matrix &A):EOMS(),g(9.81){}

blimp_EOMS::blimp_EOMS() : EOMS(), g(9.81)
{}

blimp_EOMS::~blimp_EOMS(){}

Matrix blimp_EOMS::operator ()(const double &t, const horizon::util::Matrix &y)
{
	double Density = y.getValue(5,2);
	double ZonalInitVel = y.getValue(2,1);
	double MeridionalInitVel = y.getValue(2,2);
	double AltInitVel = y.getValue(2,3);
	double Blimp_Lat = y.getValue(1,1);
	double Blimp_Long = y.getValue(1,2);
	double Blimp_Alt = y.getValue(1,3);
	double Zonal = y.getValue(3,1);
	double Meridional = y.getValue(3,2);
	double kVisc = y.getValue(5,1);
	double blimpLength = y.getValue(6,1);
	double blimpDiam = y.getValue(6,2);
	double AltVel = y.getValue(2,3);
	double mpayload = y.getValue(4,1);
	double Mgas = y.getValue(4,2);
	double blimpVol = .512*blimpDiam*blimpDiam*blimpLength;
	double Mblimp = y.getValue(4,3);
	double mSys = y.getValue(7,3);
	
	double F_Alt = (AltVel*mSys)/t;

	double rhoGas = Density-(((F_Alt/g)+mSys)/blimpVol);
	double volGas = mSys/rhoGas;
	double volAir = blimpVol - volGas;

	double rho0 = Mgas/blimpVol;
	double massOfBlimp = Mgas+Mblimp;
	double msys = mpayload+massOfBlimp;
    double relativeBlimpMass = (1-(Density/rhoGas))*massOfBlimp;



	double Thrust = y.getValue(5,3);
	double FntA = .5*(4*M_PI*(.5*blimpDiam)*(.5*blimpDiam));
	double SideA = .5*((2*M_PI*(.5*blimpDiam))*blimpLength);
	double lat_target_dist = y.getValue(7,1);
	double long_target_dist = y.getValue(7,2);
	
	
	
	
	double JD = y.getValue(3,3);
	double Cdv = .295;//for blunt bullet shape From shape effects on drag
					  //http://exploration.grc.nasa.gov/education/rocket/shaped.html
	double topArea = SideA;
	double Vz = Zonal+ZonalInitVel;
	double Vm = Meridional+MeridionalInitVel;
	
	double ZonalDrag;
	double MeridionalDrag;   

	ZonalDrag = Cdv*(.5*Density*Vz*Vz*pow(blimpVol,double(2/3)));
	MeridionalDrag = Cdv*(.5*Density*Vm*Vm*pow(blimpVol,double(2/3)));

	pair<double,double>Thrusts = Thrustequations(t, ZonalDrag,MeridionalDrag, Thrust,msys, lat_target_dist, long_target_dist,ZonalInitVel,MeridionalInitVel);
	double ZonalThrust = (Thrusts.first);
	double MeridionalThrust = (Thrusts.second);

	double Thrust_Magnitude = sqrt(ZonalThrust*ZonalThrust+MeridionalThrust*MeridionalThrust);
	
	double F_Zonal = ZonalThrust + ZonalDrag;
	double F_Meridional = MeridionalThrust + MeridionalDrag;

	double ZonalVel = (F_Zonal/mSys)*t;
	double MeridionalVel = (F_Meridional/mSys)*t;

	double LongDisp = .5*ZonalVel*t;
	double LatDisp = .5*MeridionalVel*t;
	double AltDisp = .5*AltVel*t;

	if (abs(AltVel)>20)
		cout<<"oops"<<endl;

	if (abs(ZonalVel)>50)
		cout<<"oops"<<endl;

	if (abs(MeridionalVel)>50)
		cout<<"oops"<<endl;


	Matrix LLA(3,1,0.0);
	LLA.setValue(1,1,Blimp_Lat);
	LLA.setValue(2,1,Blimp_Long);
	LLA.setValue(3,1,Blimp_Alt);
	Matrix Blimp_ECI(1,3,0.0);
	Blimp_ECI = LLA2ECI(LLA,JD);
	double X = Blimp_ECI.getValue(1,1);
	double Y = Blimp_ECI.getValue(2,1);
	double Z = Blimp_ECI.getValue(3,1);
	double recalcx = X+LongDisp;
	double recalcy = Y+LatDisp;
	double recalcz = Z+AltDisp;
	Matrix ECI(3,1,0.0);
	ECI.setValue(1,1,recalcx);
	ECI.setValue(2,1,recalcy);
	ECI.setValue(3,1,recalcz);
	Matrix Blimp_LLA = ECI2LLA(ECI,JD);
	double newLat = Blimp_LLA.getValue(1,1);
	double newLong = Blimp_LLA.getValue(1,2);
	double newAlt = Blimp_LLA.getValue(1,3);
	Matrix current_state(6,3,0.0);
	current_state.setValue(1,1,newLat);
	current_state.setValue(2,1,ZonalVel);
	current_state.setValue(3,1,Zonal);
	current_state.setValue(1,2,newLong);
	current_state.setValue(2,2,MeridionalVel);
	current_state.setValue(2,3,Meridional);
	current_state.setValue(1,3,newAlt);
	current_state.setValue(2,3,AltVel);
	current_state.setValue(5,2,Density);
	current_state.setValue(6,1,FntA);
	current_state.setValue(6,3,blimpVol);
	current_state.setValue(4,2,msys);
	current_state.setValue(5,3,Thrust_Magnitude);
	current_state.setValue(6,1,blimpLength);
	current_state.setValue(6,2,blimpDiam);
	return current_state;
};
//==========================================================================================================================
pair<double,double> blimp_EOMS::Thrustequations(double t,double ZonalDrag, double MeridionalDrag, double Thrust,double msys, double Lat_dist, double Long_dist, double ZonalInitVel,double MeridionalInitVel)const
{
				double ZonalThrust;
				double MeridionalThrust;

				double Lat_dist_mag = abs(Lat_dist);
				double Long_dist_mag = abs(Long_dist);

				//double a = atan((Lat_dist_mag/Long_dist_mag));
				double a = atan((Lat_dist/Long_dist));
				//double b = tan(Lat_dist_mag/Long_dist_mag);
				//double c = M_PI-(a+b);
				

				

				if (Lat_dist_mag<.1 && Long_dist_mag <.1)
				{
					//ZonalThrust = (msys*ZonalInitVel)/t+ZonalDrag;
					ZonalThrust = -ZonalDrag;
					//MeridionalThrust = (msys*MeridionalInitVel)/t+MeridionalDrag;
					MeridionalThrust = -MeridionalDrag;
				}

				else if (Lat_dist_mag>.1 && Long_dist_mag<.1) 
				{

					//ZonalThrust = (msys*ZonalInitVel)/t+ZonalDrag;
					ZonalThrust = -ZonalDrag;
					if(Lat_dist<0)
						MeridionalThrust = -(Thrust*sin(a));
					else
						MeridionalThrust = Thrust*sin(a);

					/*
					for(MeridionalThrust;abs((MeridionalThrust/MeridionalDrag))<1;)
					{
						if(MeridionalThrust>0)
							MeridionalThrust=MeridionalThrust+10;
						else
							MeridionalThrust=MeridionalThrust-10;
					}
					*/

				}
				else if (Lat_dist_mag<.1 && Long_dist_mag>.1)
				{
					if(Long_dist<0)
						ZonalThrust = -(Thrust*cos(a));
					else
						ZonalThrust = Thrust*cos(a);
					MeridionalThrust = -MeridionalDrag;
					
					/*
					for(ZonalThrust;abs((ZonalThrust/ZonalDrag))<1;)
					{
						if(ZonalThrust>0)
							ZonalThrust=ZonalThrust+10;
						else
							ZonalThrust=ZonalThrust-10;
		
					}
					*/
				}

				else
				{	
					if(Long_dist<0)
						ZonalThrust = -(Thrust*cos(a));
					else
						ZonalThrust = Thrust*cos(a);
					if(Lat_dist<0)
						MeridionalThrust = -(Thrust*sin(a));
					else
						MeridionalThrust = Thrust*sin(a);
					
					/*
					for(MeridionalThrust;abs((MeridionalThrust/MeridionalDrag))<1;)
					{
						if(MeridionalThrust>0)
							MeridionalThrust=MeridionalThrust+10;
						else
							MeridionalThrust=MeridionalThrust-10;	
					}
					*/
							
				}

				pair<double,double> Thrusts (ZonalThrust,MeridionalThrust);
				return Thrusts;
}


	}
	}
}
