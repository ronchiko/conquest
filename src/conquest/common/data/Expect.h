#pragma once

#include <istream>
#include <sstream>

namespace conquest {

template<typename T>
class StreamExpect
{
public:
	constexpr StreamExpect(T value)
		: m_Value(value)
	{}

	void read(std::istream& stream) const
	{
		T value;
		stream >> value;

	    if(m_Value != value) {
			std::stringstream message;
			message << "Got unexpected value (Got: " << value << ", expected: " << m_Value << ")";
			throw std::runtime_error(message.str());
		}
	}

private:
	T m_Value;
};

template<typename T>
constexpr StreamExpect<T> expect(T value)
{
	return StreamExpect<T>(std::move(value));
}

}

template<typename T>
std::istream& operator>>(std::istream& stream, const conquest::StreamExpect<T>& expected)
{
	expected.read(stream);
	return stream;
}
