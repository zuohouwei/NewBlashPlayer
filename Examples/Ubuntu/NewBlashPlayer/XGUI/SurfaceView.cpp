//
// Created by parallels on 9/8/18.
//

#include "SurfaceView.hpp"
#include "Types.hpp"
#include "Event.hpp"
#include "Widget.hpp"
#include "Painter.hpp"

#include <string>
#include <iostream>
#include <assert.h>

namespace Xgui {

    SurfaceView::SurfaceView(Widget *parent, int bg, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h, flags)
    {
        _init(bg);
    }

    SurfaceView::SurfaceView(Widget *parent, int bg, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size, flags)
    {
        _init(bg);
    }

    SurfaceView::SurfaceView(Widget *parent, int bg, const Rect &rect, Flags flags)
        : Widget(parent, rect, flags)
    {
        _init(bg);
    }

    void SurfaceView::_init(int bg) {
        _bg = bg;

        Widget::create();

//        char green[] ="#00FF00";
//        XColor color;
//        Colormap colormap = DefaultColormap(Xgui::display, 0);
//        XParseColor(Xgui::display, colormap, green, &color);
//
//        XSetWindowBackground(Xgui::display, _X11Window, color.pixel);
    }

}