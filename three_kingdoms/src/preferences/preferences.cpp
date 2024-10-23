#include "preferences/preferences.h"

#include "video.h"

#include <sys/stat.h> // for setting the permissions of the preferences file


#ifdef _WIN32
#include <windows.h>
#endif

prefs::prefs()
{

}

prefs::~prefs()
{

}

bool prefs::auto_pixel_scale()
{
	return true;
}

void prefs::set_auto_pixel_scale(bool choice)
{
	
}

int prefs::pixel_scale()
{
	// For now this has a minimum value of 1 and a maximum of 4.
    int pixel_scale = 1;
	return pixel_scale;
}

void prefs::set_pixel_scale(const int scale)
{
	
}