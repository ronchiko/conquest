#include <sstream>

#include "conquest/Error.h"

#include "conquest/engine/ui/event/ConquestEvent.h"


namespace conquest::window::event
{

namespace
{

constexpr auto CONQUEST_EVENT_MAGIC = 'CNQT';

}

ConquestEvent::ConquestEvent(const EventType type)
	: m_Type(type)
{}


std::optional<ConquestEvent> ConquestEvent::parse(const ftxui::Event& event)
{
	if(event.is_character() || event.is_cursor_position() || event.is_cursor_shape() || event.is_mouse()) {
		return {};
	}

	std::istringstream stream(event.input());

	uint32_t magic;
	stream >> magic;

	if(magic != CONQUEST_EVENT_MAGIC) {
		return {};
	}

	uint16_t type;
	stream >> type;

	return ConquestEvent(static_cast<EventType>(type));
}

ftxui::Event ConquestEvent::serialize()
{
	std::ostringstream stream;
	stream << CONQUEST_EVENT_MAGIC << static_cast<uint16_t>(m_Type);

	return ftxui::Event::Special(stream.str());
}

}
