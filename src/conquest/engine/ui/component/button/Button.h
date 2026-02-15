#pragma once

#include <ftxui/component/component.hpp>


namespace conquest::ui {

ftxui::Component button(const std::string& text, const std::function<void()>& action);

/**
	Create a button that is only clickable if a condition is met.

	@param text - The text to display on the button.
	@param action - The action to perform when the button is clicked.
	@param condition - The condition that must be true for the button to be clickable.
 */
ftxui::Component conditionalButton(
	const std::string& text, 
	const std::function<void()>& action, 
	const std::function<bool()>& condition
);

}
