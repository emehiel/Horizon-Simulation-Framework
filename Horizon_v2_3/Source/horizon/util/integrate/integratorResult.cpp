#include "integratorResult.h"

namespace horizon {
	namespace util {
		namespace integrate {

integratorResult::integratorResult(void) {}

integratorResult::~integratorResult(void) {}
/*
integratorResult & integratorResult::operator= ( const integratorResult & other )
{

	return *this;
}
*/
ostream & operator << (ostream &os, integratorResult &ort)
{
	map<double, Matrix>::iterator mapIt = ort.ODE_RESULT.begin();
	map<double, Matrix>::iterator lastMapIt = ort.ODE_RESULT.end();

    for (; mapIt != lastMapIt; ++mapIt)
    {
		os << mapIt->first << "\t" << transpose(mapIt->second) << endl;
    }

	return os;
}

}}} // end namespace horizon::util::integrate