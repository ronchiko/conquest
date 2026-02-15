#pragma once

#include <ftxui/component/component.hpp>


namespace conquest::ui {

namespace detail {

inline ftxui::Component spacer(const size_t size, const ftxui::Constraint constraint, const ftxui::WidthOrHeight dimension) {
	return ftxui::Renderer([=] {
		auto element = ftxui::filler() | ftxui::size(dimension, constraint, static_cast<int32_t>(size));
		if(constraint != ftxui::EQUAL) {
			element |= ftxui::flex;
		}
		return element;
	});
}

}

inline ftxui::Component hspacer(const std::optional<size_t> size, const ftxui::Constraint constraint = ftxui::EQUAL) {
	if(!size.has_value()) {
		return detail::spacer(1, ftxui::Constraint::GREATER_THAN, ftxui::WIDTH);    
	}

	return detail::spacer(size.value(), constraint, ftxui::WIDTH);
}

inline ftxui::Component vspacer(const std::optional<size_t> size, const ftxui::Constraint constraint = ftxui::EQUAL)
{
	if(!size.has_value()) {
		return detail::spacer(1, ftxui::Constraint::GREATER_THAN, ftxui::HEIGHT);
	}

	return detail::spacer(size.value(), constraint, ftxui::HEIGHT);
}

}
