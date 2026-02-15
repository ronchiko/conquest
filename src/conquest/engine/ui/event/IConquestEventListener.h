#pragma once

#include <ftxui/component/component.hpp>

#include "conquest/engine/ui/event/ConquestEvent.h"
#include "conquest/engine/ui/component/IComponent.h"

namespace conquest::window::event {

class IConquestEventListener : virtual public component::IComponent
{
public:
	virtual bool onEvent(const ConquestEvent& event) = 0;

private:
	[[nodiscard]] bool OnEvent(ftxui::Event baseEvent) final {
		if(const auto event = ConquestEvent::parse(baseEvent); event.has_value()) {
			return onEvent(*event);
		}

		return false;
	}
};

}
