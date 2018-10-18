#include "Widget.hpp"
#include "Window.hpp"
#include "XApp.hpp"

namespace Xgui
{
    Window::Window(Widget *parent, const String &title, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h, flags)
    {
        _init(title);
    }

    Window::Window(Widget *parent, const String &title, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size)
    {
        _init(title);
    }

    Window::Window(Widget *parent, const String &title, const Rect &rect, Flags flags)
        : Widget(parent, rect)
    {
        _init(title);
    }

    void Window::_init(const String &title)
    {
        mTitle = title;

        //create the video
        create();

        char green[] ="#FF00FF";
        XColor color;
        Colormap colormap = DefaultColormap(xUIGetApp()->getDisplay(), 0);
        XParseColor(xUIGetApp()->getDisplay(), colormap, green, &color);
        XAllocColor(xUIGetApp()->getDisplay(), colormap, &color);

        XSetWindowBackground(xUIGetApp()->getDisplay(), _X11Window, color.pixel);

//        XSetForeground(Xgui::display, _X11GC, color.pixel);

//        XSetBackground(Xgui::display, _X11GC, color.pixel);
    }

    bool Window::create() {
        Widget::create();

        if (!xUIGetApp()->getTopLevelWindow()) {
            xUIGetApp()->setTopLevelWindow(this);
        }

        XSelectInput(xUIGetApp()->getDisplay(), _X11Window, ExposureMask|ButtonPressMask|ButtonReleaseMask|
                                                            StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

        XStoreName(xUIGetApp()->getDisplay(), _X11Window, mTitle.c_str());
    }
}