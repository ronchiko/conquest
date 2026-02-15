#pragma once

#include <chrono>

namespace conquest {

class Timer
{
public:
	using Duration_t = std::chrono::milliseconds;
	using TimePoint_t = std::chrono::time_point<std::chrono::steady_clock, Duration_t>;

	explicit Timer(Duration_t durationMs);

	/**
		Waits for the timer to finish waiting.
	 */
	void wait() const;

	/**
		Checks if the timer expired
	 */
	[[nodiscard]] bool expired() const;

	/**
		Resets to timer
	 */
	void reset();

private:
	static TimePoint_t now();

	TimePoint_t m_StartPoint;
	Duration_t m_Duration;
};

}