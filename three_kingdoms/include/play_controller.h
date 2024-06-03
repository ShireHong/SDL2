

#pragma once

#include <set>
#include "controller_base.h"
#include "mouse_events.h"

class play_controller : public controller_base
{
public:
	play_controller();
	virtual ~play_controller();
    	
	//virtual void play_slice(bool is_delay_enabled = true);
    events::mouse_handler& get_mouse_handler_base() override;

    
	void play_slice_catch();
protected:


    events::mouse_handler mouse_handler_;
};
