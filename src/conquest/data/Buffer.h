#pragma once

#include <string_view>
#include <vector>

namespace conquest {

class Buffer
{
public:
	explicit Buffer(size_t size);

	[[nodiscard]] constexpr size_t length() const;

	[[nodiscard]] constexpr uint8_t *address();
	[[nodiscard]] constexpr const uint8_t *address() const;

	[[nodiscard]] std::string_view asStringView() const;

private:
	std::vector<uint8_t> m_Content;
};

constexpr size_t Buffer::length() const
{
	return m_Content.size();
}

constexpr uint8_t* Buffer::address()
{
	return m_Content.data();
}

constexpr const uint8_t* Buffer::address() const
{
	return m_Content.data();
}

}
