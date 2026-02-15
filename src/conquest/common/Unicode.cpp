#include "conquest/common/Unicode.h"


namespace conquest {
std::pair<int32_t, int32_t> parseUnicodeString(const char *stream)
{
	const auto *current = reinterpret_cast<const uint8_t *>(stream);
	if(*current < 0x80) {
		return std::make_pair(static_cast<uint32_t>(*current), 1);
	}

	uint32_t character = 0, consume = 0;
	if((*current & 0xE0) == 0xC0) {
		character = (*current & 0x1F);
		consume = 1;
	} else if((*current & 0xF0) == 0xE0) {
		character = (*current & 0x0F);
		consume = 2;
	} else if((*current & 0xF8) == 0xF0) {
		character = (*current & 0x07);
		consume = 3;
	}

	++current;
	for(; consume > 0; --consume, ++current) {
		character = (character << 6) | (*current & 0x3F);
	}

	return std::make_pair(character, static_cast<uint32_t>(reinterpret_cast<const char *>(current) - stream));
}


}
