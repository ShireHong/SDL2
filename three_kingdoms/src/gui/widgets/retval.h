
#pragma once

namespace gui2
{
/**
 * Default window/dialog return values.
 *
 * These values are named return values and most are assigned to a widget
 * automatically when using a certain id for that widget (ie, a widget with
 * the id "ok" will be assigned the OK return value.
 *
 * The automatic return values are always a negative number.
 */
enum retval : int
{
	/** Default, unset return value. */
	NONE = 0,

	/** Dialog was closed with the OK button. */
	OK = -1,

	/** Dialog was closed with the CANCEL button. */
	CANCEL = -2,

	/** The dialog was closed automatically as its timeout had been reached. */
	AUTO_CLOSE = -3,
};

} // namespace gui2
