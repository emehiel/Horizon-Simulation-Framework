#include "Asset.h"

namespace horizon {

Asset::Asset(Position* assetPosition) : assetPosition(assetPosition) {}

Asset::~Asset() { delete assetPosition; }

} // end namespace::horizon
