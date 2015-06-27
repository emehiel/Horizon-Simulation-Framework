#ifndef ODE_H
#define ODE_H

#include <functional>
#include <vector>
#include <map>
#include "..\Matrix.h"
#include "..\eoms\EOMS.h"
#include "integratorResult.h"
#include "integratorArgs.h"

namespace horizon {
	namespace util {
		namespace integrate {

using namespace std;
using std::vector;
using horizon::util::eoms::EOMS;

class Integrator
{
	integratorArgs args;

public:
	Integrator(void);

	integratorResult rk4(EOMS* inFunction, Matrix tSpan, Matrix y0);
	integratorResult rk45(EOMS* inFunction, Matrix tSpan, Matrix y0);
	Matrix trapz(vector<Matrix> mVec, double dt);
	double trapz(vector<double> mVec, double dt);

	/**
	 * Gets an integratorArgs parameter by name
	 * @param param the name of the parameter
	 * @return the parameter
	 */
	double getParam(const string param) const;

	/**
	 * Sets an integratorArgs parameter by name
	 * @param param the name of the parameter to set
	 * @param paramValue the value of the parameter
	 */
	void setParam(const string param, const double paramValue) ;
};

}}} // end namespace horizon::util::integrate

#endif // ODE_H
