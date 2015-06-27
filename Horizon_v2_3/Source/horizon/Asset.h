/**
 * \file Asset.h
 * This file contains the Asset class.
 * @author Cory O'Connor
 * @author Brian Butler (documentation)
 */

#ifndef ASSET_H_
#define ASSET_H_
#include "Asset.h"
#include "Subsystem.h"
#include "geom/Position.h"
#include "horizon/io/xmlParser.h"

namespace horizon {

	using horizon::geom::Position;

/**
 * A collection of Subsystems with a Position.
 * @author Cory O'Connor
 * @author Brian Butler (documentation)
 */
class Asset
{
	Position* assetPosition;

	/// \todo TODO: Make isTaskable mean something... like it can only canExtend!
	bool isTaskable;

public:

	/**
	 * A constructor for the Asset class.
	 * Creates an Asset using the specified Position.
	 * @param assetPosiiton A pointer to the Position object
	 * that will be used for this object.
	 */
	Asset(Position* assetPosition);

	/**
	 * A constructor for the Asset class.
	 * Creates an Asset using the specified XMLNode.
	 * @param assetXMLNode The XMLNode that the Asset will be created from.
	 * The XMLNode must contain a child <CODE><POSITION></CODE> XMLNode
	 * from which the Position object for this Asset will be created.
	 */
	Asset(XMLNode& assetXMLNode);

	/**
	 * A destructor for the Asset class.
	 * This destructor will delete the Position object used by this Asset.
	 */
	~Asset();

	/**
	 * Gets the Position of the Asset.
	 * @return A pointer to the Position object for this Asset.
	 */
	Position* getPos() const;

	/**
	 * Returns whether the Asset is taskable.
	 * @return true if the Asset is taskable, false otherwise.
	 */
	const bool getIsTaskable() const;
};

inline const bool Asset::getIsTaskable() const { return isTaskable; }

inline Position* Asset::getPos() const { return assetPosition; }

} // end namespace horizon

#endif /*ASSET_H_*/
