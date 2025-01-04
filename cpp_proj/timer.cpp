#include "timer.h"
#include <sgg/graphics.h>

/**
 * @brief Constructs a new Timer object.
 *
 * Initializes the Timer with the specified period and type. Sets the initial state of the timer
 * to not running and not paused.
 *
 * @param period The duration of the timer in seconds.
 * @param type The type of timer behavior (e.g., once, looping, ping-pong).
 */
Timer::Timer(float period, timer_type_t type)
	: m_period(period), m_type(type)
{
	// Constructor body can be expanded if additional initialization is required
}

/**
 * @brief Starts the Timer.
 *
 * Records the current global time as the start time, and sets the timer state to running
 * and not paused.
 */
void Timer::start()
{
	m_time_start = graphics::getGlobalTime() / 1000.f; // Convert milliseconds to seconds
	m_pause = false;
	m_running = true;
}

/**
 * @brief Stops the Timer.
 *
 * Sets the timer state to not running and not paused, effectively halting any ongoing timing.
 */
void Timer::stop()
{
	m_pause = false;
	m_running = false;
}

/**
 * @brief Pauses or resumes the Timer.
 *
 * Allows the Timer to be paused or resumed based on the provided boolean flag. If the Timer is
 * resumed from a paused state, it recalculates the start time to account for the elapsed time
 * before the pause.
 *
 * @param p A boolean flag indicating whether to pause (true) or resume (false) the Timer.
 */
void Timer::pause(bool p)
{
	if (!m_running)
		return;

	m_pause = p;

	if (!m_pause)
	{
		switch (m_type)
		{
		case TIMER_ONCE:
		case TIMER_LOOPING:
			// Adjust start time based on the current value and period to resume accurately
			m_time_start = graphics::getGlobalTime() / 1000.f - m_val * m_period;
			break;
		case TIMER_PINGPONG:
			// Adjust start time for ping-pong behavior, accounting for ascending or descending phase
			m_time_start = graphics::getGlobalTime() / 1000.f - m_period * (m_pingpong_descending ? (2.0f - m_val) : m_val);
			break;
		}
	}
}

/**
 * @brief Converts the Timer object to a float representing the current timer value.
 *
 * Implements the float conversion operator to allow the Timer object to be used in contexts
 * where a float value is expected. Updates the timer's current value based on its type and
 * the elapsed time since it started.
 *
 * @return The current value of the timer as a float between 0.0f and 1.0f.
 */
Timer::operator float()
{
	if (m_running && !m_pause)
	{
		switch (m_type)
		{
		case TIMER_ONCE:
			// Calculate the progress of a single-run timer
			m_val = std::min(1.0f, (graphics::getGlobalTime() / 1000.f - m_time_start) / m_period);
			if (m_val == 1.0f)
			{
				m_running = false;
				m_pause = false;
			}
			break;
		case TIMER_LOOPING:
			// Calculate the progress of a looping timer using modulo for continuous cycling
			m_val = fmodf(graphics::getGlobalTime() / 1000.f - m_time_start, m_period) / m_period;
			break;
		case TIMER_PINGPONG:
			// Calculate the progress of a ping-pong timer, reversing direction at each end
			m_val = fmodf(graphics::getGlobalTime() / 1000.f - m_time_start, 2.0f * m_period) / m_period;
			m_pingpong_descending = m_val > 1.0f;
			m_val = (m_val <= 1.0f ? m_val : 2.0f - m_val);
			break;
		}
	}

	return m_val;
}
