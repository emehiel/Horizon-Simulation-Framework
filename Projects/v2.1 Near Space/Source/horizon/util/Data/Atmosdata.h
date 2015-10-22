#ifndef ATMOSDATA_H
#define ATMOSDATA_H

namespace horizon{
	namespace util{
		namespace Data{
/*
This Class defines the atmospheric properties for each level of altitude. The data is loaded from
a program written by Carmichel in the Station Class.
*/

class Atmosdata
{
private:

	double Density;
	double Pressure;
	double Temperature;
	double Viscosity;

public:
	
	Atmosdata (){};
	void setvalues(double pressure, double density, double Temperature, double viscosity);
	double writepressure();
	double writedensity();
	double writetemp();
	double writevis();
	~Atmosdata (){};
};
		}
	}
}
#endif // ATMOSDATA_H
