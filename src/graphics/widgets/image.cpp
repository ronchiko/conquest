#include "conquest/graphics/widgets/image.h"

namespace conquest::gui {

	Image::Image(v2<uint32> origin, const std::string& path, const Color foreground, const Color background)
		: mOrigin(origin)
		, mAsset(Asset::load(path, AssetType::Art))
		, mForeground(foreground)
		, mBackground(background)
	{}

	void Image::draw(Screen& screen, bool)
	{
		screen.draw(mOrigin, mAsset, mForeground, mBackground);
	}
}