#include <thread>

#include "conquest/common/Timer.h"

namespace conquest {

Timer::Timer(const Duration_t durationMs)
	: m_StartPoint(now())
	, m_Duration(durationMs)
{}

void Timer::reset()
{
	m_StartPoint = now();
}

void Timer::wait() const
{
	std::this_thread::sleep_for(m_Duration - (now() - m_StartPoint));
}

bool Timer::expired() const
{
	const auto now = Timer::now();
	return now - m_StartPoint > m_Duration;
}

Timer::TimePoint_t Timer::now()
{
	using Clock_t = std::chrono::steady_clock;
	using Ratio_t = std::ratio_divide<Clock_t::period, Duration_t::period>;

	return TimePoint_t(Duration_t(Clock_t::now().time_since_epoch().count() * Ratio_t::num / Ratio_t::den));
}

}
