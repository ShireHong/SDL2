
/**
 * @file
 * Defines the exception classes for the layout algorithm.
 */

#pragma once

#include <exception>

namespace gui2
{

/**
 * Exception thrown when the width has been modified during resizing.
 *
 * See layout_algorithm for more information.
 */
struct layout_exception_width_modified : public std::exception
{
};

/** Basic exception when the layout doesn't fit. */
struct layout_exception_resize_failed : public std::exception
{
};

/**
 * Exception thrown when the width resizing has failed.
 *
 * See layout_algorithm for more information.
 */
struct layout_exception_width_resize_failed
		: public layout_exception_resize_failed
{
};

/**
 * Exception thrown when the height resizing has failed.
 *
 * See layout_algorithm for more information.
 */
struct layout_exception_height_resize_failed
		: public layout_exception_resize_failed
{
};

} // namespace gui2
