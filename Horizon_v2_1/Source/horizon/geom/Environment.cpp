#include "Environment.h"

namespace horizon {
	namespace geom {

		Environment::Environment() : sun(new Sun(false)){}

		Environment::Environment(Sun* sun): sun(sun){}

		Environment::~Environment() { delete sun; }

}}  // end namespace horizon::geom