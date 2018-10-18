#ifndef XGUI_BUTTON_HPP
#define XGUI_BUTTON_HPP
#include "Widget.hpp"
#include "Types.hpp"
#include "Event.hpp"

#include <string>


namespace Xgui
{
    class Button : public Widget
    {
    public:
        Callback clicked;

        Button(Widget *parent, const String &label, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        Button(Widget *parent, const String &label, const Point &pos, const Size &size, Flags flags = NoFlags);
        Button(Widget *parent, const String &label, const Rect &rect, Flags flags = NoFlags);
        inline String label(void) const { return _label; }
        inline void setLabel(const String &label) { _label = label; }
        inline bool value(void) const { return _value; }
    
    protected:
        virtual void paintEvent(PaintEventData &);
        virtual void mousePressEvent(MouseEventData &event);
        virtual void mouseMoveEvent(MouseEventData &event);
        virtual void mouseReleaseEvent(MouseEventData &event);

    private:
        String _label;
        bool _value;
        
        void _init(const String &label, Flags);
    };
}

#endif