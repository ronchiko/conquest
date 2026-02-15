#pragma once

#include <chrono>
#include <optional>

#include <ftxui/component/animation.hpp>

namespace conquest::window {

constexpr auto DEFAULT_TICK_DURATION = std::chrono::milliseconds(50);


class Ticker
{
public:
	using OnTickCallback_t = std::function<void()>;

	explicit Ticker(
		OnTickCallback_t callback,
		std::optional<size_t> tickCount = {}, 
		ftxui::animation::Duration tickDuration = DEFAULT_TICK_DURATION
	);

	void tick();
	void restart(std::optional<size_t> tickCount = {});

private:
	OnTickCallback_t m_Callback;
	std::optional<size_t> m_TickCount;
	
	ftxui::animation::TimePoint m_LastCheckTime;
	ftxui::animation::Duration m_TickDuration;
};

}
