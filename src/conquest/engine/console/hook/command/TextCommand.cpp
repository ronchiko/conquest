#include "conquest/common/data/Expect.h"
#include "conquest/log/Log.h"

#include "conquest/engine/console/hook/command/TextCommand.h"


namespace conquest::engine::console {

inline constexpr auto RESET_COLORS_COMMAND = 0;

inline constexpr auto MULTICOLOR_FOREGROUND_COMMAND = 38;
inline constexpr auto MULTICOLOR_BACKGROUND_COMMAND = 48;

inline constexpr Color COLOR_MAPPING[] = {
	/* 0 = */ { 0, 0, 0 },
	/* 1 = */ { 255, 0, 0 },
	/* 2 = */ { 0, 255, 0 },
	/* 3 = */ { 255, 255, },
	/* 4 = */ { 0, 0, 255 },
	/* 5 = */ { 255, 0, 255 },
	/* 6 = */ { 0, 255, 255 },
	/* 7 = */ { 255, 255, 255 },
};

void TextCommand::execute(std::istringstream& stream, TextWriter& writer)
{
	uint32_t commandId = 0;
	stream >> commandId;

	if(commandId == RESET_COLORS_COMMAND) {
		writer.setForeground(DEFAULT_FOREGROUND);
		writer.setBackground(DEFAULT_BACKGROUND);
		return;
	}

	if(commandId == MULTICOLOR_FOREGROUND_COMMAND || commandId == MULTICOLOR_BACKGROUND_COMMAND) {
		int32_t subCommandId = 0, red = 0, green = 0, blue = 0;
		stream >> expect(';') >> subCommandId;
		if(subCommandId != 2) {
			return;
		}

		stream >> expect(';') >> red >> expect(';') >> green >> expect(';') >> blue >> expect('m');
		const auto color = Color{ static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue) };

		if(commandId == MULTICOLOR_BACKGROUND_COMMAND) {
			writer.setBackground(color);
			return;
		}

		writer.setForeground(color);
		return;
	} 

	if(30 <= commandId && commandId <= 37) {
		writer.setForeground(COLOR_MAPPING[commandId - 30]);
		return;
	}

	if(40 <= commandId && commandId <= 47) {
		writer.setBackground(COLOR_MAPPING[commandId - 40]);
		return;
	}

	if(49 == commandId) {
		writer.setBackground(DEFAULT_BACKGROUND);
		return;
	}

	if(39 == commandId) {
		writer.setForeground(DEFAULT_FOREGROUND);
		return;
	}

	log::error("Unknown text command")
	    .param("id", commandId);
}


}
