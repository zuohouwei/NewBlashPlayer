#include "ScrollBar.hpp"
#include "Widget.hpp"
#include "ScrollArea.hpp"

#include <iostream>
#include <cmath>


namespace Xgui
{
    ScrollBar::ScrollBar(Widget *parent, ScrollArea *scrollarea, int x, int y, int w, int h)
        : Widget(parent, x, y, w, h)
        , _scrollarea(scrollarea)
    {

    }

    void ScrollBar::paintEvent(PaintEventData &)
    {
        Painter painter(this);
        painter.clear();

        double proportion = (double)_rect.h()/(double)_scrollarea->realHeight();
        if (proportion > 1.0) {
            proportion = 1.0;
        }

        int handle_height = round(_rect.h() * proportion);
        if (handle_height < 4) handle_height = 4;

        proportion = (double)(-_scrollarea->offset().y())/(double)_scrollarea->realHeight();
        if (proportion > 1.0) {
            proportion = 1.0;
        }

        int handle_y = round(_rect.h() * proportion);
        painter.fillRect(0, handle_y, _rect.w(), handle_height);
    }
}