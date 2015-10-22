#ifndef WINDATA_H
#define WINDATA_H

namespace horizon{
	namespace util{
		namespace Data{


class Windata
{
private:
	double Zonal_Component;
	double Meridional_Component;
	

public:

	Windata () {};
	void setvalues(double zonal_component, double meridional_component);
	double writezonal_component();
	double writemeridional_component();
	~Windata (){};
};
		}}}

#endif
