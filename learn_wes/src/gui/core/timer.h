
#pragma once

#include <functional>

#include <SDL2/SDL_types.h>

namespace gui2
{

/**
 * Adds a new timer.
 *
 * @param interval                The timer interval in ms.
 * @param callback                The function to call when the timer expires,
 *                                the id send as parameter is the id of the
 *                                timer.
 * @param repeat                  If true the timer will restart after it
 *                                expires.
 *
 * @returns                       The id of the timer.
 * @retval [0]                    Failed to create a timer.
 */
std::size_t add_timer(const uint32_t interval,
				 const std::function<void(std::size_t id)>& callback,
				 const bool repeat = false);

/**
 * Removes a timer.
 *
 * It's save to remove a timer in its own callback, only the value returned
 * might not be accurate. The destruction is postponed until the execution is
 * finished  and the return value is whether the postponing was successful.
 *
 * @param id                      The id of the timer to remove, this is the id
 *                                returned by add_timer.
 *
 * @returns                       Status, false if the timer couldn't be
 *                                removed.
 */
bool remove_timer(const std::size_t id);

/**
 * Executes a timer.
 *
 * @note this function is only meant to be executed by the event handling
 * system.
 *
 * @param id                      The id of the timer to execute, this is the
 *                                id returned by add_timer.
 *
 * @returns                       Status, false if the timer couldn't be
 *                                executed.
 */
bool execute_timer(const std::size_t id);

} // namespace gui2
