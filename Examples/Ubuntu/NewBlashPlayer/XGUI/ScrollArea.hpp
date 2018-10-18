#ifndef XGUI_SCROLLAREA_HPP
#define XGUI_SCROLLAREA_HPP
#include "Widget.hpp"
#include "Types.hpp"
#include "ScrollBar.hpp"


namespace Xgui
{
    class ScrollArea : public Widget
    {
    public:
        ScrollArea(Widget *, int, int, int, int);
        virtual inline size_t realHeight(void) const { return 0; }
        void setVerticalScrollBar(ScrollBar *);
        inline ScrollBar *verticalScrollBar(void) const { return _verticalScrollBar; }
        inline virtual Point offset(void) const { return _offset; }

    protected:
        ScrollBar *_verticalScrollBar;
        Point _offset;
    };
}

#endif