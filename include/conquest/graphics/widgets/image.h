#pragma once

#include "conquest/graphics/asset.h"
#include "conquest/graphics/screen.h"
#include "conquest/graphics/widgets/widget.h"

namespace conquest::gui {
	class Image : public IWidget
	{
	public:
		explicit Image(v2<uint32> origin, const std::string& asset, Color foreground, Color background = Color::Black);

		virtual void draw(Screen& screen, bool);

	private:
		v2<uint32> mOrigin;
		Asset mAsset;
		Color mForeground;
		Color mBackground;
	};

}