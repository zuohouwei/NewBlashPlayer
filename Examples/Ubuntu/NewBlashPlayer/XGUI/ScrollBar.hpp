#ifndef XGUI_SCROLLBAR_HPP
#define XGUI_SCROLLBAR_HPP
#include "Widget.hpp"
#include "Event.hpp"


namespace Xgui
{
    class ScrollArea;
    class ScrollBar : public Widget
    {
    public:
        ScrollBar(Widget *, ScrollArea *, int, int, int, int);
        inline void setScrollArea(ScrollArea *scrollarea) { _scrollarea = scrollarea; };
        
    protected:
        ScrollArea *_scrollarea; /* widget to be scrolled, not
                                  * necessarily the parent widget */


        virtual void paintEvent(PaintEventData &);
        //virtual void scrollEvent(ScrollEventData &);
    };
}

#endif