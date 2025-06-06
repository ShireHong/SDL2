
#pragma once

#include "gui/widgets/helper.h"
#include "gui/widgets/widget.h"
#include "utils/const_clone.h"
// #include "wml_exception.h"

namespace gui2
{

/**
 * Returns the first parent of a widget with a certain type.
 *
 * @param child                   The widget to get the parent from,
 * @tparam T                      The class of the widget to return.
 *
 * @returns                       The parent widget.
 */
template <class T>
T& get_parent(widget& child)
{
	T* result;
	widget* w = &child;
	do {
		w = w->parent();
		result = dynamic_cast<T*>(w);

	} while(w && !result);

	assert(result);
	return *result;
}

/**
 * Gets a widget with the wanted id.
 *
 * This template function doesn't return a pointer to a generic widget but
 * returns the wanted type and tests for its existence if required.
 *
 * @param widget              The widget test or find a child with the wanted
 *                            id.
 * @param id                  The id of the widget to find.
 * @param must_be_active      The widget should be active, not all widgets
 *                            have an active flag, those who don't ignore
 *                            flag.
 * @param must_exist          The widget should be exist, the function will
 *                            fail if the widget doesn't exist or is
 *                            inactive and must be active. Upon failure a
 *                            wml_error is thrown.
 *
 * @returns                   The widget with the id.
 */
template <class T>
T* find_widget(utils::const_clone_ptr<widget, T> widget,
			   const std::string& id,
			   const bool must_be_active,
			   const bool must_exist)
{
	T* result = dynamic_cast<T*>(widget->find(id, must_be_active));
	// VALIDATE(!must_exist || result, missing_widget(id));

	return result;
}

/**
 * Gets a widget with the wanted id.
 *
 * This template function doesn't return a reference to a generic widget but
 * returns a reference to the wanted type
 *
 * @param widget              The widget test or find a child with the wanted
 *                            id.
 * @param id                  The id of the widget to find.
 * @param must_be_active      The widget should be active, not all widgets
 *                            have an active flag, those who don't ignore
 *                            flag.
 *
 * @returns                   The widget with the id.
 */
template <class T>
T& find_widget(utils::const_clone_ptr<widget, T> widget,
			   const std::string& id,
			   const bool must_be_active)
{
	return *find_widget<T>(widget, id, must_be_active, true);
}

template<typename T>
void on_widget(utils::const_clone_ptr<widget, T> parent, const std::string& id, std::function<void(T&)> func)
{
	if(auto widget = find_widget<T>(parent, id, false, false)) {
		func(*widget);
	}
}
} // namespace gui2
