#include "Screen.hpp"

#include <X11/Xlib.h>


namespace Xgui
{
    Screen::Screen(Display *display)
        : _display(display)
    {
    }
}