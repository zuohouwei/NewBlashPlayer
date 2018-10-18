#include "Widget.hpp"
#include "Types.hpp"
#include "FontXft.hpp"
#include "Painter.hpp"
#include "XApp.hpp"

#include <X11/Xlib.h>
#include <iostream>

namespace Xgui
{
    Widget::Widget(Widget *parent, int x, int y, size_t w, size_t h, Flags flags)
        : _parent(parent)
        , _rect(x, y, w, h)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    Widget::Widget(Widget *parent, const Point &pos, const Size &size, Flags flags)
        : _parent(parent)
        , _rect(pos, size)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    Widget::Widget(Widget *parent, const Rect &rect, Flags flags)
        : _parent(parent)
        , _rect(rect)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    void Widget::_init(Flags flags)
    {
        _color = Color::fromRGB(229, 229, 229);
        _doubleBuffered = false;

        if (flags & DoubleBuffered) {
            /* window is double buffered */
            _doubleBuffered = true;
        }
    }

    bool Widget::create() {
        ::Window parentWindow;
        if (_parent) {
            _parent->addChild(this);
            parentWindow = _parent->X11Window();
        } else {
            parentWindow = DefaultRootWindow(xUIGetApp()->getDisplay());
        }

        std::cout << "The rect x : " << _rect.x()
                  << "y : " << _rect.y()
                  << "w : " << _rect.w()
                  << "h : " << _rect.h()
                  << std::endl;

        _X11Window = XCreateSimpleWindow(xUIGetApp()->getDisplay(),
                                         parentWindow,
                                         _rect.x(),
                                         _rect.y(),
                                         _rect.w(),
                                         _rect.h(),
                                         0,
                                         WhitePixel(xUIGetApp()->getDisplay(), 0),
                                         WhitePixel(xUIGetApp()->getDisplay(), 0));

//        XSelectInput(xUIGetApp()->getDisplay(), _X11Window, ExposureMask|ButtonPressMask|ButtonReleaseMask|
//                                                StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

        Atom WM_DELETE_WINDOW = XInternAtom(xUIGetApp()->getDisplay(), "WM_DELETE_WINDOW", False);
        XSetWMProtocols(xUIGetApp()->getDisplay(), _X11Window, &WM_DELETE_WINDOW, 1);

        _X11GC = XCreateGC(xUIGetApp()->getDisplay(), _X11Window, 0, NULL);

//        XSetForeground(Xgui::display, _X11GC, 0);

//        XClearWindow(Xgui::display, _X11Window);
//        XMapRaised(Xgui::display, _X11Window);

        xUIGetApp()->addWidget(this);
    }

    void Widget::addChild(Widget *child)
    {
        _children.push_back(child);
    }

    void Widget::redraw(void)
    {
        /* Force an expose event on the widget */
        PaintEventData d = {0};
        paintEvent(&d);

        for(std::vector<Widget*>::const_iterator cit = _children.begin();
                cit != _children.end(); ++cit)
        {
            (*cit)->redraw();
        }
    }

    void Widget::show(void)
    {
        XMapWindow(xUIGetApp()->getDisplay(), _X11Window);

        for(std::vector<Widget*>::const_iterator cit = _children.begin();
            cit != _children.end(); ++cit)
        {
            (*cit)->show();
        }
    }

    void Widget::hide(void)
    {
        for(std::vector<Widget*>::const_iterator cit = _children.begin();
            cit != _children.end(); ++cit)
        {
            (*cit)->hide();
        }

        XUnmapWindow(xUIGetApp()->getDisplay(), _X11Window);
    }

    void Widget::event(Event *event)
    {
        switch (event->type) {
            case MousePress:
                mousePressEvent(event->mousedata);
                break;
            case MouseMove:
                mouseMoveEvent(event->mousedata);
                break;
            case MouseRelease:
                mouseReleaseEvent(event->mousedata);
                break;
            case Scroll:
                scrollEvent(event->scrolldata);
                break;
            case KbdPress:
                keyPressEvent(event->keyboarddata);
                break;
            case KbdRelease:
                keyReleaseEvent(event->keyboarddata);
                break;
            case Paint:
                paintEvent(event->paintdata);
                break;
            default:
                break;
        }
    }
}
