#ifndef XGUI_WINDOW_HPP
#define XGUI_WINDOW_HPP

#include "Widget.hpp"
#include "Types.hpp"

#include <string>


namespace Xgui
{
    class Window : public Widget
    {
    public:
        Window(Widget *parent, const String &title, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        Window(Widget *parent, const String &title, const Point &pos, const Size &size, Flags flags = NoFlags);
        Window(Widget *parent, const String &title, const Rect &rect, Flags flags = NoFlags);

    private:
        void _init(const String &title);

        bool create();

    private:
        String mTitle;
    };
}

#endif