
#pragma once

#include "gui/core/widget_definition.h"
#include "gui/core/window_builder.h"

#include "gui/widgets/styled_widget.h"
#include "gui/widgets/clickable_item.h"

namespace gui2
{
namespace implementation
{
	struct builder_button;
}

// ------------ WIDGET -----------{

/**
 * Simple push button.
 */
class button : public styled_widget, public clickable_item
{
public:
	explicit button(const implementation::builder_button& builder);

	/***** ***** ***** ***** Inherited ***** ***** ***** *****/

	/** See @ref styled_widget::set_active. */
	virtual void set_active(const bool active) override;

	/** See @ref styled_widget::get_active. */
	virtual bool get_active() const override;

	/** See @ref styled_widget::get_state. */
	virtual unsigned get_state() const override;

	/** Inherited from clickable_item. */
	virtual void connect_click_handler(const event::signal& signal) override
	{
		connect_signal_mouse_left_click(*this, signal);
	}

	/** Inherited from clickable_item. */
	virtual void disconnect_click_handler(const event::signal& signal) override
	{
		disconnect_signal_mouse_left_click(*this, signal);
	}

	/***** ***** ***** setters / getters for members ***** ****** *****/

	void set_retval(const int retval)
	{
		retval_ = retval;
	}

private:
	/**
	 * Possible states of the widget.
	 *
	 * Note the order of the states must be the same as defined in settings.hpp.
	 */
	enum state_t {
		ENABLED,
		DISABLED,
		PRESSED,
		FOCUSED,
	};

	void set_state(const state_t state);
	/**
	 * Current state of the widget.
	 *
	 * The state of the widget determines what to render and how the widget
	 * reacts to certain 'events'.
	 */
	state_t state_;

	/**
	 * The return value of the button.
	 *
	 * If this value is not 0 and the button is clicked it sets the retval of
	 * the window and the window closes itself.
	 */
	int retval_;

public:
	/** Static type getter that does not rely on the widget being constructed. */
	static const std::string& type();

private:
	/** Inherited from styled_widget, implemented by REGISTER_WIDGET. */
	virtual const std::string& get_control_type() const override;

	/***** ***** ***** signal handlers ***** ****** *****/

	void signal_handler_mouse_enter(const event::ui_event event, bool& handled);

	void signal_handler_mouse_leave(const event::ui_event event, bool& handled);

	void signal_handler_left_button_down(const event::ui_event event,
										 bool& handled);

	void signal_handler_left_button_up(const event::ui_event event,
									   bool& handled);

	void signal_handler_left_button_click(const event::ui_event event,
										  bool& handled);
};

// }---------- DEFINITION ---------{

struct button_definition : public styled_widget_definition
{
	explicit button_definition(const config& cfg);

	struct resolution : public resolution_definition
	{
		explicit resolution(const config& cfg);
	};
};

// }---------- BUILDER -----------{

class styled_widget;

namespace implementation
{

struct builder_button : public builder_styled_widget
{
public:
	explicit builder_button(const config& cfg);

	using builder_styled_widget::build;

	virtual std::unique_ptr<widget> build() const override;

private:
	std::string retval_id_;
	int retval_;
};

} // namespace implementation

// }------------ END --------------

} // namespace gui2
