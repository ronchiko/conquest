#pragma once

#include <functional>
#include <ftxui/component/component.hpp>

#include "conquest/engine/ui/component/button/Choice.h"


namespace conquest::ui {

namespace detail {

template<typename T>
struct ReflectiveButtonCreator;

template<typename T>
struct ReflectiveButtonCreator<ui::Choice<T>>
{
	static ftxui::Component create(
		Choice& component,
	    const std::function<void()>& action,
		const std::string& fallbackText
	) {
		return component.makeReflectiveButton(action, fallbackText);
	}
};

}

template<typename T>
ftxui::Component reflect(T& component, const std::function<void()>& action) {
	return detail::ReflectiveButtonCreator<T>::create(component, action, "Not chosen");
}

}
