#include "conquest/graphics/widgets/selection.h"

#include <algorithm>

namespace conquest::gui {

	Selection::Selection(Area rectangle, std::vector<std::string> options, const SelectionSettings& settings)
		: mState({})
		, mSettings(settings)
		, mArea(rectangle)
		, mOptions(std::move(options))
	{}

	void Selection::draw(Screen& screen, bool highlighted)
	{
		// Draw the selection outline.
		screen.draw(mArea,
					mSettings.outline.background,
					mSettings.outline.pattern,
					highlighted ? mSettings.outline.highlight : mSettings.outline.foreground);

		const size_t maxOptionLength = mArea.size.x - 4; // 2 for the outlines and 2 to space the out.
		const size_t optionCount = (mArea.size.y - 4) / mSettings.spacing;

		// For each object that is visible draw it.
		uint32 currentY = mArea.point.y + 2;
		for(size_t index = 0; index <= optionCount; ++index) {
			const auto offset = mState.topItem + index;

			// If index is out of the range of mOptions we should stop drawing the file.
			if(index >= mOptions.size()) {
				return;
			}

			std::string option = mOptions[mState.topItem + index];

			// Clamp the options string length.
			if(option.size() > maxOptionLength) {
				option = option.substr(0, maxOptionLength);
			}

			// Center each option entry.
			const uint32 start = mArea.point.x + static_cast<uint32>((mArea.size.x - option.size()) / 2);
			screen.draw(v2<uint32>{ start, currentY },
						option,
						// Highlight the selected item.
						offset == mState.pointer ? mSettings.highlight : mSettings.normalColor,
						mSettings.background);
			currentY += mSettings.spacing;
		}
	}

	bool Selection::sendInput(const InputResult& event)
	{
		const auto optionCount = static_cast<int32>((mArea.size.y - 4) / mSettings.spacing);
		switch(event.type) {
		case InputType::Down:
			mState.pointer++;
			break;
		case InputType::Up:
			mState.pointer--;
			break;
		default:
			return false; // In this case we did not consume the input.
		}
		mState.pointer = std::clamp(mState.pointer, 0, static_cast<int32>(mOptions.size() - 1));

		// Make mState.pointer is not too far away from mState.topItem
		if(mState.pointer - mState.topItem > optionCount) {
			mState.topItem = mState.pointer - optionCount;
		}
		// Ensure our top item is always at least pointer.
		if(mState.pointer < mState.topItem) {
			mState.pointer = mState.pointer;
		}

		return true;
	}
}