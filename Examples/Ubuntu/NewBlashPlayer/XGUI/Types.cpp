#include "Types.hpp"


namespace Xgui
{
    /* Xgui::Point */
    Point::Point(void)
        : _x(0)
        , _y(0)
    {
    }

    Point::Point(int x, int y)
        : _x(x)
        , _y(y)
    {
    }

    Point::Point(const Point& point)
    {
        _x = point.x();
        _y = point.y();
    }

    /* Xgui::Size */
    Size::Size(void)
        : _w(0)
        , _h(0)
    {
    }

    Size::Size(size_t w, size_t h)
        : _w(w)
        , _h(h)
    {
    }

    Size::Size(const Size& size)
        : _w(size.w())
        , _h(size.h())
    {
    }

    /* Xgui::Rect */
    Rect::Rect(void)
        : _x(0)
        , _y(0)
        , _w(0)
        , _h(0)
    {
    }

    Rect::Rect(int x, int y, size_t w, size_t h)
        : _x(x)
        , _y(y)
        , _w(w)
        , _h(h)
    {
    }

    Rect::Rect(const Point &pos, const Size &size)
        : _x(pos.x())
        , _y(pos.y())
        , _w(size.w())
        , _h(size.h())
    {
    }

    Rect::Rect(const Rect &rect)
        : _x(rect.x())
        , _y(rect.y())
        , _w(rect.w())
        , _h(rect.h())
    {
    }

    /* Xgui::Color */
    Color::Color(void)
    {
        _init(0, 0, 0, 0xffff);
    }
    
    Color::Color(unsigned short r, unsigned short g, unsigned short b)
    {
        _init(r, g, b, 0xffff);
    }

    Color Color::fromRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        unsigned short proportion = 0xffff/0xff;
        return Color(proportion * r, proportion * g, proportion * b);
    }

    void Color::_init(unsigned short r, unsigned short g, unsigned short b, unsigned short a)
    {
        _r = r; _g = g; _b = b; _a = a;
    }
}