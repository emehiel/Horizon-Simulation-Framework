#ifndef XMLDATAWRITER_H
#define XMLDATAWRITER_H

#include <string>
#include <list>
#include <vector>
#include <set>
#include <sstream>
#include "../../systemSchedule.h"
//#include "../../util/Matrix.h"
//#include "../../../simParams.h"
#include "../../System.h"
#include "xmlNode.h"
#include "AssetNode.h"
#include "SubsystemDataNode.h"

namespace horizon {
namespace io {
namespace xml {

	using horizon::State;

class xmlDataWriter {

public:
	xmlDataWriter(void);
	
	~xmlDataWriter();
	
	bool writeSchedule(systemSchedule* schedule, System* system, const string filename);

};
}}} //end namespace horizon::io::xml
#endif