
#pragma once

#include <type_traits>

namespace utils
{
/**
 * Helper struct to clone the constness of one type to another.
 *
 * @warning It seems @c *this in a const member function is not a const object,
 * use @c this, which is a pointer to a const object.
 *
 * @tparam D                      The destination type, it should have no
 *                                cv-qualifier and not be a pointer or
 *                                reference.
 * @tparam S                      The source type, this type may be a pointer
 *                                or reference and obviously is allowed to have
 *                                a cv-qualifier, although @c volatile has no
 *                                effect.
 */
template<typename D, typename S>
struct const_clone
{
	static const bool is_source_const =
		std::is_const_v<
			std::remove_pointer_t<
				std::remove_reference_t<S>
			>
		>;

	/** The destination type, possibly const qualified. */
	using type =
		std::conditional_t<is_source_const, const D, D>;

	/** A reference to the destination type, possibly const qualified. */
	using reference =
		std::conditional_t<is_source_const, const D&, D&>;

	/** A pointer to the destination type, possibly const qualified. */
	using pointer =
		std::conditional_t<is_source_const, const D*, D*>;
};

template<typename D, typename S>
using const_clone_t = typename const_clone<D, S>::type;

template<typename D, typename S>
using const_clone_ref = typename const_clone<D, S>::reference;

template<typename D, typename S>
using const_clone_ptr = typename const_clone<D, S>::pointer;

} // namespace utils
