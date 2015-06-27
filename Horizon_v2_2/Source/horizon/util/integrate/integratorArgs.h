#ifndef ODE_ARGS_H
#define ODE_ARGS_H

#include <limits>
#include <string>
#include <iostream>

namespace horizon {
	namespace util {
		namespace integrate {
		
	using std::numeric_limits;
	using std::string;
	using std::cout;
	using std::endl;

class integratorArgs
{
	/** initial guess for h - delta t */
	double h;

	/** relative tolerance used by the integrator solver. */
	double rtol;

	/** absolute tolerance used by the integrator solver, (important when solution is close to zero). */
	double atol;

	/** a small number */
	double eps;

public:
	/**
	 * Creates a new integratorArgs object with default values:
	 * h = 0.1
	 * rtol = 1e-3
	 * atol = 1e-6
	 * eps = numeric_limits<double>::epsilon( );
	 */
	integratorArgs(void);

	/**
	 * Creates a new integratorArgs object with user defined values:
	 * @param h the initial step size
	 * @param rtol the relative tolerance
	 * @param atol the absolute tolerance
	 * @param eps the eps value (a small number)
	 */
	integratorArgs(const double h, const double rtol, const double atol, const double eps);

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

#endif // ODE_ARGS_H //