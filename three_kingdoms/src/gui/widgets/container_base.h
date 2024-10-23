
#pragma once

// #include "gui/widgets/grid.hpp"
#include "gui/widgets/styled_widget.h"

namespace gui2
{

/**
 * A generic container base class.
 *
 * A container is a class build with multiple items either acting as one
 * widget.
 *
 */
class container_base : public styled_widget
{
	friend class debug_layout_graph;

public:
	explicit container_base(const implementation::builder_styled_widget& builder, const std::string& control_type);

	/**
	 * Returns the client rect.
	 *
	 * The client rect is the area which is used for child items. The rest of
	 * the area of this widget is used for its own decoration.
	 *
	 * @returns                   The client rect.
	 */
	virtual SDL_Rect get_client_rect() const;

	/***** ***** ***** ***** layout functions ***** ***** ***** *****/

	/** See @ref widget::layout_initialize. */
	virtual void layout_initialize(const bool full_initialization) override;

	/**
	 * Tries to reduce the width of a container.
	 *
	 * See @ref layout_algorithm for more information.
	 *
	 * @param maximum_width       The wanted maximum width.
	 */
	void reduce_width(const unsigned maximum_width);

	/** See @ref widget::request_reduce_width. */
	virtual void request_reduce_width(const unsigned maximum_width) override;

	/** See @ref widget::demand_reduce_width. */
	virtual void demand_reduce_width(const unsigned maximum_width) override;

	/**
	 * Tries to reduce the height of a container.
	 *
	 * See @ref layout_algorithm for more information.
	 *
	 * @param maximum_height      The wanted maximum height.
	 */
	void reduce_height(const unsigned maximum_height);

	/** See @ref widget::request_reduce_height. */
	virtual void request_reduce_height(const unsigned maximum_height) override;

	/** See @ref widget::demand_reduce_height. */
	virtual void demand_reduce_height(const unsigned maximum_height) override;

protected:
	/** See @ref widget::calculate_best_size. */
	virtual point calculate_best_size() const override;

public:
	/** See @ref widget::can_wrap. */
	virtual bool can_wrap() const override;

	/** See @ref widget::place. */
	virtual void place(const point& origin, const point& size) override;

	/***** ***** ***** ***** Inherited ***** ***** ***** *****/

	/** See @ref widget::has_widget. */
	virtual bool has_widget(const widget& widget) const override;

	/** See @ref widget::set_origin. */
	virtual void set_origin(const point& origin) override;

	/** See @ref widget::set_visible_rectangle. */
	virtual void set_visible_rectangle(const SDL_Rect& rectangle) override;

	/** See @ref widget::impl_draw_children. */
	virtual void impl_draw_children() override;

protected:
	/** See @ref widget::layout_children. */
	virtual void layout_children() override;

public:
	/** See @ref widget::find_at. */
	virtual widget* find_at(const point& coordinate,
							 const bool must_be_active) override;

	/** See @ref widget::find_at. */
	virtual const widget* find_at(const point& coordinate,
								   const bool must_be_active) const override;

	/** See @ref widget::find. */
	widget* find(const std::string& id, const bool must_be_active) override;

	/** See @ref widget::find. */
	const widget* find(const std::string& id,
						const bool must_be_active) const override;

	/** See @ref styled_widget::set_active. */
	virtual void set_active(const bool active) override;

	/** See @ref widget::disable_click_dismiss. */
	//bool disable_click_dismiss() const override;

	/**
	 * See @ref widget::create_walker.
	 */
	// virtual iteration::walker_ptr create_walker() override;

	/**
	 * Initializes and builds the grid.
	 *
	 * This function should only be called upon an empty grid.
	 *
	 * @param grid_builder        The builder for the grid.
	 */
	// void init_grid(const builder_grid& grid_builder);

	/***** **** ***** ***** wrappers to the grid **** ********* *****/


	void set_child(std::unique_ptr<widget> widget,
				   const unsigned row,
				   const unsigned col,
				   const unsigned flags,
				   const unsigned border_size);

private:
	/** The grid which holds the child objects. */

	/** Returns the space used by the border. */
	virtual point border_space() const;

	/**
	 * Helper for set_active.
	 *
	 * This function should set the styled_widget itself active. It's called by
	 * set_active if the state needs to change. The widget is set to dirty() by
	 * set_active so we only need to change the state.
	 */
	virtual void set_self_active(const bool active) = 0;

	void inject_linked_groups();
};

} // namespace gui2
