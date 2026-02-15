#include "conquest/engine/console/hook/command/ToggleCommand.h"

#include "conquest/common/data/Expect.h"
#include "conquest/log/Log.h"


namespace conquest::engine::console {

namespace {

constexpr auto TOGGLE_LINE_WARP = 7;
constexpr auto TOGGLE_CURSOR_VISIBLE = 25;
constexpr auto TOGGLE_MOUSE_X10_EVENTS = 1000;
constexpr auto TOGGLE_ALL_MOUSE_EVENTS = 1003;
constexpr auto TOGGLE_SGR_MOUSE_MODE = 1006;
constexpr auto TOGGLE_URXVT_MOUSE_MODE = 1015;

void handleToggle(std::istringstream& stream, bool mode)
{
	uint32_t toggleVersion = 0;
	stream >> expect('?') >> toggleVersion;

	switch(toggleVersion) {
	case TOGGLE_LINE_WARP:
	case TOGGLE_CURSOR_VISIBLE:
	case TOGGLE_MOUSE_X10_EVENTS:
	case TOGGLE_ALL_MOUSE_EVENTS:
	case TOGGLE_SGR_MOUSE_MODE:
	case TOGGLE_URXVT_MOUSE_MODE:
		return; // All these all enabled by default in the terminal emulation.

	default:
		log::warn("Unknown toggle event").param("toggle id", toggleVersion);
		return;
	}
}

}

void SetToggleCommand::execute(std::istringstream &stream, TextWriter &writer)
{
	handleToggle(stream, true);
}

void ResetToggleCommand::execute(std::istringstream &stream, TextWriter &writer)
{
	handleToggle(stream, false);
}


}
