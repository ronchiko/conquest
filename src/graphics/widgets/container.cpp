#include "conquest/graphics/widgets/container.h"

#include <algorithm>

constexpr conquest::int32 NON_FOCUSABLE = -1;

namespace conquest::gui {

	void Container::add(IWidget& widget)
	{
		add(std::make_pair(NON_FOCUSABLE, &widget));
	}

	void Container::draw(Screen& screen, bool highlighted)
	{
		for(const auto& [focusIndex, widget] : mWidgets) {
			widget->draw(screen, highlighted && focusIndex == mPointer && NON_FOCUSABLE != focusIndex);
		}
	}

	bool Container::sendInput(const InputResult& input)
	{
		if(InputType::Enter == input.type) {
			return increment();
		}

		// If the subobject is an IInputWidget, try to send it the input.
		if(auto *inputWidget = dynamic_cast<IInputWidget *>(mFocusList[mPointer])) {
			// See if the contained widget consumed the input, and if so
			// stop running.
			if(inputWidget->sendInput(input)) {
				return true;
			}
		}

		return false;
	}

	void Container::add(std::pair<int32, IWidget *> widget)
	{
		mWidgets.push_back(widget);
	}

	bool Container::increment()
	{
		if(mFocusList.size() - 1 <= mPointer) {
			return false;
		}

		++mPointer;
		return true;
	}

	bool Container::decrement()
	{
		if(mPointer <= 0) {
			return false;
		}

		mPointer--;
		return true;
	}
}
