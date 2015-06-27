#include "Asset.h"

namespace horizon {

Asset::Asset(Position* assetPosition) : assetPosition(assetPosition) {}

Asset::Asset(XMLNode& assetXMLNode) : assetPosition(new Position(assetXMLNode.getChildNode("POSITION"))) {}

Asset::~Asset() { delete assetPosition; }

} // end namespace::horizon
