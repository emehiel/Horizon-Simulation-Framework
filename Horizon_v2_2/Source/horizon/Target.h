#ifndef TARGET_H
#define TARGET_H

#include <string>
#include "geom/geomUtil.h"
#include "util/Matrix.h"
#include "geom/Position.h"
#include "horizon/io/xmlParser.h"

namespace horizon {

using std::string;
using horizon::geom::Position;
using horizon::util::Matrix;
using horizon::io::XMLNode;
using horizon::util::transpose;
using horizon::util::norm;

/**
 * A named location denoting where a task is to be performed.
 * @author Einar Pehrson
 */
class Target {

	/** The name of the target */
	string name;

	/** The type of the target */
	string targetType;

protected:

	/** The position of the target */
	Position* position;

	/** The value of the target */
	int value;

	int minQualCM;
	
	int freq_days;
	string CC;
	string WX_Reg;
	string Comment;

public:

	/**
	 * Creates a new target from the xmlNode data
	 * @param targetNode the xmlNode which contains the relevant target information
	 */
	Target(XMLNode targetXMLNode);

	/**
	 * Creates a new target with the given name and position.
	 * @param name a name describing the target
	 * @param position the position of the target
	 */
	Target(const string& name, Position* position);

	/**
	 * Creates a new target with the given name, position and value.
	 * @param name a name describing the target
	 * @param position the position of the target
	 * @param value the value of the target
	 */
	Target(const string& name, Position* position, int value);

	/** Returns the name of the target. */
	const string& getName() const;

	/** Returns the type of the target. */
	const string& getType() const;

	/** Returns the value of the target. */
	const int getValue() const;

	/**
	 * Returns the position of the target at the given time in LLA.
	 * @param time the time at which the target is to be located
	 * @return the position of the target at the given time
	 */
	Position* getPos() const;

	// USER - Add other accessor function here with inline definitions below;

	const int getMinQualCM() const;
	const int getFreq_days() const;
	const string getCC() const;
	const string getWX_Reg() const;
	const string getComment() const;

	/** Compares the name of this target with that of another */
	bool operator==(const Target& other) const;
};

inline const string& Target::getName() const {
	return name;
}

inline const string& Target::getType() const {
	return targetType;
}

inline const int Target::getValue() const {
	return value;
}

inline const int Target::getMinQualCM() const {
	return minQualCM;
}

inline const int Target::getFreq_days() const {
	return freq_days;
}

inline const string Target::getCC() const {
	return CC;
}

inline const string Target::getWX_Reg() const {
	return WX_Reg;
}

inline const string Target::getComment() const {
	return Comment;
}

} // end namespace horizon

#endif /*TARGET_H_*/
