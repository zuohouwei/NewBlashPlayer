#ifndef XGUI_SCREEN_HPP
#define XGUI_SCREEN_HPP
#include <X11/Xlib.h>


namespace Xgui
{
    class Screen
    {
    public:
        Screen(Display *);
        inline ::Screen *XScreen(void) { return DefaultScreenOfDisplay(_display); }
    
    private:
        Display *_display;
    };
}

#endif