

#pragma once

#include "gui/core/static_registry.h"

/**
 * Registers a widget.
 *
 * Call this function to register a widget. Use this macro in the
 * implementation, inside the gui2 namespace.
 *
 * See @ref gui2::register_widget for more information.
 *
 * @note When the type is foo_definition, the id "foo" and no special key best
 * use RESISTER_WIDGET(foo) instead.
 *
 * @param type                    Class type of the window to register.
 * @param id                      Id of the widget
 * @param key                     The id to load if differs from id.
 */
#define REGISTER_WIDGET3(type, id, key)                                                                                \
	namespace                                                                                                          \
	{                                                                                                                  \
	namespace ns_##type##id                                                                                            \
	{                                                                                                                  \
		struct register_helper                                                                                         \
		{                                                                                                              \
			register_helper()                                                                                          \
			{                                                                                                          \
                                                                                                                       \
 			}                                                                                                          \
		};                                                                                                             \
                                                                                                                       \
		static struct register_helper register_helper;                                                                 \
	}                                                                                                                  \
	}

/**
 * Wrapper for REGISTER_WIDGET3.
 *
 * "Calls" REGISTER_WIDGET3(id_definition, id, nullptr)
 */
#define REGISTER_WIDGET(id)                                                                                            \
	REGISTER_WIDGET3(id##_definition, id, nullptr)                                                                     \
                                                                                                                       \
	const std::string& id::type()                                                                                      \
	{                                                                                                                  \
		static const std::string result(#id);                                                                          \
		return result;                                                                                                 \
	}                                                                                                                  \
                                                                                                                       \
	const std::string& id::get_control_type() const                                                                    \
	{                                                                                                                  \
		return id::type();                                                                                             \
	}
