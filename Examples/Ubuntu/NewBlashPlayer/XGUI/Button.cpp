#include "Button.hpp"
#include "Types.hpp"
#include "Event.hpp"
#include "Widget.hpp"
#include "Painter.hpp"

#include <string>
#include <iostream>
#include <assert.h>


namespace Xgui
{
    Button::Button(Widget *parent, const String &label, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h)
    {
        _init(label, flags);
    }

    Button::Button(Widget *parent, const String &label, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size)
    {
        _init(label, flags);
    }

    Button::Button(Widget *parent, const String &label, const Rect &rect, Flags flags)
        : Widget(parent, rect)
    {
        _init(label, flags);
    }

    void Button::paintEvent(PaintEventData &)
    {
        Painter painter(this);

        Rect rect = _font.textExtents(_label);
        
        int x = (_rect.w() - rect.w())/2;
        int y = (_rect.h() + _font.lineHeight())/2;

        if (_value) {
            /* draw button pressed down */
            painter.setColor(Color(0, 0, 0));
            painter.fillRect(Point(), _rect.size());
            //painter.drawArc(0, 0, 10, 10, );

            painter.setTextColor(Color(0xffff, 0xffff, 0xffff));
            painter.drawText(_label, x, y - _font.descent());
        } else {
            /* draw button normal */
            painter.setColor(Color(0xffff, 0xffff, 0xffff));
            painter.fillRect(Point(), _rect.size());

            painter.setTextColor(Color(0, 0, 0));
            painter.drawText(_label, x, y - _font.descent());
        }
    }

    void Button::mousePressEvent(MouseEventData &event)
    {
        if (event.button == Left) {
            _value = true;
            redraw();
        }
    }

    void Button::mouseMoveEvent(MouseEventData &)
    {

    }

    void Button::mouseReleaseEvent(MouseEventData &event)
    {
        if (event.button == Left) {
            _value = false;
            redraw();

            if (clicked) {
                /* call the callback */
                clicked(this);
            }
        }
    }

    void Button::_init(const String &label, Flags)
    {
        _label = label;
        _value = false;
    }
}