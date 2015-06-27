#ifndef ASSET_H_
#define ASSET_H_
#include "Asset.h"
#include "Subsystem.h"
#include "geom/Position.h"
#include "horizon/io/xmlParser.h"

namespace horizon {

	using horizon::geom::Position;

/**
 * A collection of subsystems with a position.
 * @author Cory O'Connor
 */
class Asset
{
	Position* assetPosition;

	/// \todo TODO: Make isTaskable mean something... like it can only canExtend!
	bool isTaskable;

public:

	Asset(Position* assetPosition);

	Asset(XMLNode& assetXMLNode);

	~Asset();

	Position* getPos() const;

	const bool getIsTaskable() const;
};

inline const bool Asset::getIsTaskable() const { return isTaskable; }

inline Position* Asset::getPos() const { return assetPosition; }

} // end namespace horizon

#endif /*ASSET_H_*/
