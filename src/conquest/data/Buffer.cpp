#include "conquest/data/Buffer.h"

namespace conquest {

Buffer::Buffer(const size_t size)
{
	m_Content.resize(size);
}

std::string_view Buffer::asStringView() const
{
	return { reinterpret_cast<const char *>(address()), length() };
}

}
