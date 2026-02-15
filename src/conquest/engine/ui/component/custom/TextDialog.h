#pragma once

#include <string>
#include <sstream>

#include <ftxui/component/component.hpp>

#include "conquest/engine/ui/event/IConquestEventListener.h"
#include "conquest/engine/ui/component/IAnimatedComponent.h"
#include "conquest/engine/ui/Ticker.h"


namespace conquest::window::component {

class TextDialogComponent : public IAnimatedComponent,
							public event::IConquestEventListener
{
public:
	explicit TextDialogComponent(std::string text);

	ftxui::Element render() override;

	void animate(ftxui::animation::Duration duration) override;

	bool onEvent(const event::ConquestEvent& event) override;

private:
	void onTick();

	Ticker m_Ticker;
	std::vector<std::stringstream> m_Lines;
	size_t m_Offset;
	std::string m_Text;
};

ftxui::Component dialog(const std::string& text);

}
