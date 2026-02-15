#pragma once

#include <optional>
#include <type_traits>
#include <sstream>

#include <ftxui/component/event.hpp>


namespace conquest::window::event {

enum class EventType : uint16_t
{
	WindowStart,
};

class ConquestEvent
{
public:
	explicit ConquestEvent(EventType type);

	static std::optional<ConquestEvent> parse(const ftxui::Event& event);

	[[nodiscard]] constexpr EventType type() const {
		return m_Type;
	}

	ftxui::Event serialize();

private:

	EventType m_Type;
};

}
