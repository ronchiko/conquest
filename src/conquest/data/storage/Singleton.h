#pragma once

#include <memory>

#include "conquest/Error.h"

namespace conquest {

template<typename T>
class Singleton
{
public:
	/**
		Gets the instance of the singleton or initializes it if its not initialized yet.
	 */
	template<typename... Ts>
	Singleton(Ts&&...params);

	/**
		Gets the instance of the singleton, if the singleton is not yet initialized, throws an ConquestError.
	 */
	template<typename = std::enable_if_t<!std::is_default_constructible_v<T>>>
	Singleton();

	constexpr T *operator->();
	constexpr const T *operator->() const;

	consteval T& operator*();
	consteval const T& operator*() const;

protected:
	inline static std::unique_ptr<T> g_Instance = nullptr;
};

template<typename T>
template<typename... Ts>
Singleton<T>::Singleton(Ts&&...params)
{
	static_assert(std::is_constructible_v<T, Ts...>);

	if(nullptr != g_Instance) {
		return;
	}

	g_Instance = std::make_unique<T>(std::forward<Ts>(params)...);
}

template<typename T>
template<typename>
Singleton<T>::Singleton()
{
	if(nullptr == g_Instance) {
		throw ConquestError("Singleton is not initialized");
	}
}

template<typename T>
constexpr T *Singleton<T>::operator->()
{
	return g_Instance.get();
}

template<typename T>
constexpr const T *Singleton<T>::operator->() const
{
	return g_Instance.get();
}

template<typename T>
consteval T& Singleton<T>::operator*()
{
	return *g_Instance;
}

template<typename T>
consteval const T& Singleton<T>::operator*() const
{
	return *g_Instance;
}

}
