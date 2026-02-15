#include "conquest/common/Finally.h"

#include "conquest/engine/ui/Ticker.h"


namespace conquest::window {

Ticker::Ticker(OnTickCallback_t callback,
			   const std::optional<size_t> tickCount,
			   const ftxui::animation::Duration tickDuration)
	: m_Callback(std::move(callback))
	, m_TickCount(tickCount)
	, m_LastCheckTime(ftxui::animation::Clock::now())
	, m_TickDuration(tickDuration)
{}

void Ticker::tick()
{
	if(m_TickCount.has_value() && m_TickCount.value() <= 0) {
		return;
	}

	const auto currentTime = ftxui::animation::Clock::now();
	if(currentTime - m_LastCheckTime > m_TickDuration) {
		m_Callback();
		m_LastCheckTime = currentTime;

		if(m_TickCount && m_TickCount.value() > 0) {
			m_TickCount.value()--;
		}
	}

	ftxui::animation::RequestAnimationFrame();
}

void Ticker::restart(const std::optional<size_t> tickCount)
{
	m_LastCheckTime = ftxui::animation::Clock::now();
	m_TickCount = tickCount;

	ftxui::animation::RequestAnimationFrame();
}

}
