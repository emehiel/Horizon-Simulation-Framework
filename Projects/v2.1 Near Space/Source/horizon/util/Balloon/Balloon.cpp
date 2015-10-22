#include "Balloon.h"	

namespace horizon{
	namespace util{
		namespace Balloon{

void balloon::setvalues(Matrix bal_pos, double payload,double diameter,double ballmass, double molar_mass)
	{ 
		BalPos = bal_pos;
		BallMass = ballmass;
		sysmass = payload+ballmass;
		//rhoGas = baldens;
		mPayload = payload;
		Molar_Mass = molar_mass;
		Max_Diameter = diameter;

	};

	double balloon::writemass()
	{
		return sysmass;
	};

	double balloon::writeMaxDiameter()
	{
		return Max_Diameter;
	};

	double balloon::writePay()
	{
		return mPayload;
	};

	double balloon::writemolmass()
	{
		return Molar_Mass;
	};

	Matrix balloon::writepos()
	{
		return BalPos;
	};


	double balloon::writeballmass()
	{
		return BallMass;
	};
		}
	}
}
