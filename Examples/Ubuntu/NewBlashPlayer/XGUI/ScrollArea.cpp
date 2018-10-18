#include "ScrollArea.hpp"
#include "ScrollBar.hpp"
#include "Widget.hpp"


namespace Xgui
{
    ScrollArea::ScrollArea(Widget *parent, int x, int y, int w, int h)
        : Widget(parent, x, y, w, h)
    {

    }

    void ScrollArea::setVerticalScrollBar(ScrollBar *scrollbar)
    {
        _verticalScrollBar = scrollbar;
        _verticalScrollBar->setScrollArea(this);
    }
}