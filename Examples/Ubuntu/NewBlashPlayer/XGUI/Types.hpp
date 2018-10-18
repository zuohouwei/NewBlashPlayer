#ifndef XGUI_TYPES_HPP
#define XGUI_TYPES_HPP
#include <cstddef>


namespace Xgui
{
    /* Xgui::Point */
    class Point
    {
    public:
        Point(void);
        Point(int x, int y);
        Point(const Point&);
        inline int x(void) const { return _x; }
        inline void setX(int x) { _x = x; }
        inline int y(void) const { return _y; }
        inline void setY(int y) { _y = y; }
        inline void setXY(int x, int y) { _x = x; _y = y; }
        inline void copyFrom(const Point &other) { _x = other.x(); _y = other.y(); }
    
    private:
        int _x, _y;
    };

    /* Xgui::Size */
    class Size
    {
    public:
        Size(void);
        Size(size_t w, size_t h);
        Size(const Size&);
        inline size_t w(void) const { return _w; }
        inline void setW(size_t w) { _w = w; }
        inline size_t h(void) const { return _h; }
        inline void setH(size_t h) { _h = h; }
    
    private:
        size_t _w, _h;
    };

    /* Xgui::Rect */
    class Rect
    {
    public:
        Rect(void);
        Rect(int x, int y, size_t w, size_t h);
        Rect(const Point &pos, const Size &size);
        Rect(const Rect &rect);

        inline int x(void) const { return _x; }
        inline void setX(int x) { _x = x; }
        inline int y(void) const { return _y; }
        inline void setY(int y) { _y = y; }
        inline size_t w(void) const { return _w; }
        inline void setW(size_t w) { _w = w; }
        inline size_t h(void) const { return _h; }
        inline void setH(size_t h) { _h = h; }
        inline Point pos(void) const { return Point(_x, _y); }
        inline void setPos(const Point &pos) { _x = pos.x(); _y = pos.y(); }
        inline Size size(void) const { return Size(_w, _h); }
        inline void setSize(const Size &size) { _w = size.w(); _h = size.h(); }
    
    private:
        int _x, _y;
        size_t _w, _h;
    };

    /* Xgui::Color */
    class Color
    {
    public:
        Color(void);
        Color(unsigned short, unsigned short, unsigned short);
        static Color fromRGB(unsigned char, unsigned char, unsigned char);
        inline unsigned short r(void) const { return _r; }
        inline unsigned short g(void) const { return _g; }
        inline unsigned short b(void) const { return _b; }
        inline unsigned short a(void) const { return _a; }

    private:
        unsigned short _r, _g, _b, _a;
        void _init(unsigned short, unsigned short, unsigned short, unsigned short);
    };
}

#endif