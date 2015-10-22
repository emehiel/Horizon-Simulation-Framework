#include "Atmosdata.h"

namespace horizon{
	namespace util{
		namespace Data{

	void Atmosdata::setvalues(double pressure, double density, double temperature, double viscosity)
	{
		Pressure = pressure;
		Density = density;
		Temperature = temperature;
		Viscosity = viscosity;
	};

	double Atmosdata::writedensity()
	{
		return Density;
	};

	double Atmosdata::writepressure()
	{
		return Pressure;
	};

	double Atmosdata::writetemp()
	{
		return Temperature;
	};

	double Atmosdata::writevis()
	{
		return Viscosity;
	};

		}
	}
}

