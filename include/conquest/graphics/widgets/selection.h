#pragma once

#include <string>
#include <vector>

#include "conquest/graphics/rect.h"
#include "conquest/graphics/widgets/widget.h"
#include "conquest/types.h"

namespace conquest::gui {

	struct SelectionSettings
	{
		SelectionSettings() = default;

		uint32 spacing = 1;							// The space between each option
		Color normalColor = Color::Grey;			// The color of normal options
		Color highlight = Color::BrightBlue;		// The color of the selected options
		Color background = Color::Black;

		struct
		{
			Color foreground = Color::Grey;
			Color background = Color::Black;
			Color highlight = Color::BrightBlue;
			RectPattern pattern = patterns::EMPTY;
		} outline;
	};

	class Selection : public IWidget,
					  public IInputWidget
	{
	public:
		/**
		 * Creates a new selection list.
		 *
		 * @param rect - The area to and style to draw the selections in.
		 * @param options - A list of text value repsentations of the options to present.
		 * @param settings - Custom options to stylize the selection box.
		 */
		explicit Selection(Area rect, std::vector<std::string> options, const SelectionSettings& settings);

		virtual void draw(Screen& screen, bool highlighted) override;

		virtual bool sendInput(const InputResult& input) override;

		/**
		 * Returns the currently selected item index.
		 */
		constexpr int32 current() const;

	private:
		struct
		{
			int32 pointer = 0; // The currently selected item.
			int32 topItem = 0; // The first item to display.
		} mState;

		Area mArea;
		SelectionSettings mSettings;
		std::vector<std::string> mOptions;
	};
}

constexpr conquest::int32 conquest::gui::Selection::current() const
{
	return mState.pointer;
}