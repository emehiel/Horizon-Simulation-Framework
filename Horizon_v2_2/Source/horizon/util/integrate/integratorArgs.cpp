#include "integratorArgs.h"

namespace horizon {
	namespace util {
		namespace integrate {

integratorArgs::integratorArgs(void): h(0.1), rtol(1e-6), atol(1e-6)
{
	eps = numeric_limits<double>::epsilon( );
}

integratorArgs::integratorArgs(const double h, const double rtol, const double atol, const double eps):
h(h), rtol(rtol), atol(atol), eps(eps) {}

double integratorArgs::getParam(string param) const
{
	char * temp = (char *)param.c_str();
	_strlwr_s(temp, param.length()+1);
	param = string(temp);

	if (param == "h")
		return h;
	else if (param == "rtol")
		return rtol;
	else if (param == "atol")
		return atol;
	else if (param == "eps")
		return eps;
	else {
		cout << "ERROR: integratorArgs::getParam(const string param): NO PARAMETER WITH THAT NAME" << endl;
		return NULL;
	}
}

void integratorArgs::setParam(string param, const double paramValue)
{
	char * temp = (char *)param.c_str();
	_strlwr_s(temp, param.length()+1);
	param = string(temp);

	if (param == "h")
		h = paramValue;
	else if (param == "rtol")
		rtol  = paramValue;
	else if (param == "atol")
		atol = paramValue;
	else if (param == "eps")
		eps = paramValue;
	else
		cout << "ERROR: integratorArgs::setParam(const string param): NO PARAMETER WITH THAT NAME" << endl;

	cout << "Integrator Parameter: " << param << " set to: " << paramValue << endl;
}



}}} // end namespace horizon::util::integrate