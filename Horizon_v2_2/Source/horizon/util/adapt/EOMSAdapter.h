#ifndef EOMSADAPTER_H
#define EOMSADAPTER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "horizon/util/eoms/EOMS.h"
#include "horizon/util/eoms/orbital_EOMS.h"
#include "horizon/util/eoms/polyEOMSInput.h"
#include "horizon/util/eoms/stateSpace_EOMS.h"
#include "horizon/io/xmlParser.h"
#include "horizon/util/Matrix.h"

using namespace std;
using namespace horizon::util::eoms;
using namespace horizon::io;
using horizon::util::Matrix;
using horizon::util::atom;

#define ORBITAL_EOMS "orbital_EOMS"
#define STATESPACE_EOMS "stateSpace_EOMS"

namespace horizon {
namespace util {
namespace adapt {

	EOMS* createEOMSObject(XMLNode&);

}}}

#endif