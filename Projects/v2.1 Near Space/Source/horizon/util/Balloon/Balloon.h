#ifndef BALLOON_H
#define BALLOON_H	

#include "horizon/util/Matrix.h"
#include "horizon/util/matrixIndex.h"
#include <math.h>

namespace horizon{
namespace util{
namespace Balloon{
/*
This Class defines the properties of the balloon.
*/



	using namespace std;
	using std::make_pair;
	using horizon::util::Matrix;
	using horizon::util::matrixIndex;

class balloon
	{
	private:
		Matrix BalPos;
		double BallMass;
		double mPayload;
		double rhoGas;
		double sysmass;
		double Molar_Mass;
		double balloonAlt;
		double Max_Diameter;
	public:
		balloon () {};
		void setvalues(Matrix bal_pos, double payload,double diameter,double ballmass, double molar_mass);
		double writeballmass();
		double writemass();
		double writeMaxDiameter();
		double writePay();
		double writemolmass();
		Matrix writepos();
		double writealt();
		~balloon () {};
	};
	
}
}
}

#endif 