#pragma once

#include <cstdint>

#include <SDL3/SDL.h>


namespace conquest::engine::sdl {

namespace detail {

template<typename>
struct SdlRectType;

// clang-format off
template<> struct SdlRectType<int32_t> { using Type_t = SDL_Rect; };
template<> struct SdlRectType<float> { using Type_t = SDL_FRect; };
// clang-format on

template<typename T>
using SdlRectType_t = typename SdlRectType<T>::Type_t;

}

template<typename T>
class Rect
{
public:
	explicit Rect(T x = 0, T y = 0, T w = 0, T h = 0)
		: m_Rect{ x, y, w, h }
	{}

	// clang-format off
	T& x() { return m_Rect.x; }
	const T& x() const { return m_Rect.x; }

	T& y() { return m_Rect.y; }
	const T& y() const { return m_Rect.y; }

	T&w() { return m_Rect.w; }
	const T& w() const { return m_Rect.w; }

	T& h() { return m_Rect.h; }
	const T& h() const { return m_Rect.h; }
	// clang-format on
	
    [[nodiscard]] operator detail::SdlRectType_t<T> *() {
		return &m_Rect;
	}

    [[nodiscard]] operator const detail::SdlRectType_t<T> *() const
	{
		return &m_Rect;
	}

private:
	detail::SdlRectType_t<T> m_Rect;
};

using RectF = Rect<float>;


}