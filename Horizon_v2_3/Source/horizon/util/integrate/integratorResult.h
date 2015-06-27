#ifndef ODE_RESULT_H
#define ODE_RESULT_H

#include <map>
#include "..\Matrix.h"

namespace horizon {
	namespace util {
		namespace integrate {

using namespace std;

class integratorResult
{
public:
	integratorResult(void);
	~integratorResult(void);

	friend ostream & operator << (ostream &os, integratorResult &ort);
//	void operator << (integratorResult &ort);
//	integratorResult & operator= ( const integratorResult & other );

	map<double, Matrix> ODE_RESULT;
};

}}} // end namespace horizon::util::integrate

#endif // ODR_RESULT_H