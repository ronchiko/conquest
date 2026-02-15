#pragma once

#include <string>
#include <ftxui/component/component.hpp>


namespace conquest::ui	 {

struct Limit
{
	size_t characters = 126;
	bool multiline = false;
};

/**
   Creates a text-box UI component.

   @param text - The string that will be edited when the text box is changed.
   @param placeholder - The placeholder text that will be displayed when the text box is empty.
   @param limit - Configuration input sanitization.
 */
ftxui::Component textBox(std::string& text, const std::string& placeholder, Limit limit = {});

}
