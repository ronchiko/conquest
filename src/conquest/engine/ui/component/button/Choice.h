#pragma once

#include <functional>
#include <ftxui/component/component.hpp>

#include "conquest/common/Convert.h"
#include "conquest/data/storage/AssetSingleton.h"


namespace conquest::ui {

namespace detail {

class Choice
{
public:
	static constexpr auto NOT_CHOSEN_YET = -1;

    explicit Choice(std::vector<std::string> options, const int32_t value = NOT_CHOSEN_YET);

	[[nodiscard]] constexpr bool didChoose() const { return NOT_CHOSEN_YET != m_Value; }
	[[nodiscard]] constexpr const std::string& value() const { return m_Options[m_Value]; }

    /**
        The index of the currently selected option.
		Returns NOT_CHOSEN_YET if no option has been selected yet.
     */
    [[nodiscard]] constexpr int32_t index() const { return m_Value; }

    /**
        Creates a menu component that allows the user to choose
		one of the available options.
     */
    ftxui::Component createMenu(ftxui::MenuOption options);

    /**
		Creates a button that reflects the current choice.
		If no choice has been made, the button will display `fallbackText`.
     */
    ftxui::Component makeReflectiveButton(
		const std::function<void()>& onClick, 
		const std::string& fallbackText
	);

private:
	std::vector<std::string> m_Options;
	int32_t m_Value;
};

}

template<typename T>
class Choice : public detail::Choice
{
public:
	explicit Choice(
		std::vector<std::string> names, 
		std::vector<std::reference_wrapper<const T>> values, 
		const int32_t value = NOT_CHOSEN_YET
	)
		: detail::Choice(std::move(names), value)
		, m_Mapping(std::move(values))
	{}

	[[nodiscard]] const T& current() const {
		return m_Mapping[index()];
	}

private:
	std::vector<std::reference_wrapper<const T>> m_Mapping;
};

namespace choice
{
/**
    Creates a choice object for a type which has a global asset singleton.
 */
template<typename T>
Choice<T> fromAssetSingleton()
{
	Singleton<conquest::detail::Mapping<T>> instance;

	auto names = conquest::toVector<std::string>(instance->names());
	auto values = conquest::toVector<std::reference_wrapper<const T>>(instance->all());

	return Choice<T>(std::move(names), std::move(values));
}

}

}
