#include "windata.h"

namespace horizon{
	namespace util{
		namespace Data{

	void Windata::setvalues(double zonal_component, double meridional_component)
	{
		Zonal_Component = zonal_component;
		Meridional_Component = meridional_component;
	};

	double Windata::writezonal_component()
	{
		return Zonal_Component;
	};

	double Windata::writemeridional_component()
	{
		return Meridional_Component;
	};
		}
	}
}