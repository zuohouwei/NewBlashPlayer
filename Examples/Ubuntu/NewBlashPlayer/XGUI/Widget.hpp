#ifndef XGUI_WIDGET_HPP
#define XGUI_WIDGET_HPP
#include "Types.hpp"
#include "Event.hpp"
#include "Xgui_defs.hpp"
#include "Painter.hpp"

#include <X11/Xlib.h>
#include <vector>
#include <cstddef>


namespace Xgui
{
    class Widget
    {
    public:
        Widget(Widget *parent, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        Widget(Widget *parent, const Point &pos, const Size &size, Flags flags = NoFlags);
        Widget(Widget *parent, const Rect &rect, Flags flags = NoFlags);
        virtual void        event(Event *);
        inline Rect         rect(void) const { return _rect; }
        inline Widget       *parent(void) { return _parent; }
        inline ::Window     X11Window(void) { return _X11Window; }
        inline ::XVisualInfo* visinfo() { return _visinfo; }
        inline ::GC         X11GC(void) { return _X11GC; }
        inline Font         font(void) const { return _font; }
        inline void         setFont(Font font) { _font = font; }
        inline Color        color(void) const { return _color; }
        inline void         setColor(const Color &color) { _color = color; }
        inline Color        textColor(void) const { return _textColor; }
        inline void         setTextColor(const Color &color) { _textColor = color; }
        inline bool         doubleBuffered(void) const { return _doubleBuffered; }
        inline void         setDoubleBuffered(bool doubleBuffered) { _doubleBuffered = doubleBuffered; }
        virtual void        redraw(void);
        virtual void        show(void);
        virtual void        hide(void);

        Callback clicked;

    protected:
        ::Window            _X11Window;
        ::XVisualInfo       *_visinfo;
        ::GC                _X11GC;
        Widget *_parent;
        Rect _rect;
        Font _font;
        Color _color;
        Color _textColor;
        bool _doubleBuffered;
        std::vector<Widget *> _children;

        void                addChild(Widget *);

        virtual void paintEvent(PaintEventData* event)
        {
        }

//        void Widget::resizeEvent(void *)
//        {
//        }

        virtual void mousePressEvent(MouseEventData* event)
        {
            std::cout << "mousePressEvent : " << event->button << std::endl;
        }

        virtual void mouseMoveEvent(MouseEventData* event)
        {
            std::cout << "mouseMoveEvent : " << event->button << std::endl;
        }

        virtual void mouseReleaseEvent(MouseEventData* event)
        {
            std::cout << "mouseReleaseEvent : " << event->button << std::endl;
        }

        virtual void keyPressEvent(KeyboardEventData* event)
        {
            std::cout << "keyPressEvent : " << event->key << std::endl;
        }

        virtual void keyReleaseEvent(KeyboardEventData* event)
        {
            std::cout << "keyReleaseEvent : " << event->key << std::endl;
        }

        virtual void scrollEvent(ScrollEventData* event)
        {
        }

    protected:
        void _init(Flags flags);

        bool create();
    };
}

#endif