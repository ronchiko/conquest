#pragma once

#include <vector>

namespace conquest {

template<typename T>
class Stream
{
public:
	explicit Stream(size_t initialSize = 12);

	[[nodiscard]] bool empty() const;

	void write(T value);

	[[nodiscard]] const T& peek() const;

	void clear();

	const T *get() const;

private:
	std::vector<T> m_Buffer;
};

template<typename T>
Stream<T>::Stream(size_t initialSize)
	: m_Buffer(0)
{
	m_Buffer.reserve(initialSize);
}

template<typename T>
bool Stream<T>::empty() const
{
	return m_Buffer.empty();
}

template<typename T>
void Stream<T>::write(T value)
{
	m_Buffer.push_back(std::move(value));
}

template<typename T>
const T& Stream<T>::peek() const
{
	return *(m_Buffer.end() - 1);
}

template<typename T>
void Stream<T>::clear()
{
	m_Buffer.clear();
}

template<typename T>
const T * Stream<T>::get() const
{
	return m_Buffer.data();
}

}
