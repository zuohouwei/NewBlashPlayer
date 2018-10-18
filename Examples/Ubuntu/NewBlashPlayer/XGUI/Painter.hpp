#ifndef XGUI_PAINTER_HPP
#define XGUI_PAINTER_HPP
#include "FontXft.hpp"
#include "Types.hpp"
#include "Xgui_defs.hpp"

//#include <X11/Xft/Xft.h>

#include <string>


namespace Xgui
{
    class Widget;
    class Painter
    {
    public:
        Painter(Widget *widget);
        ~Painter(void);
        inline void setFont(Font font) { _font = font; };
        inline Font font(void) { return _font; }

        void setColor(const Color &color);
        inline Color color(void) const { return _color; }
        void setTextColor(const Color &color);
        inline Color textColor(void) const { return _textColor; }
        inline bool doubleBuffered(void) const { return _doubleBuffered; }
        void clear(void);
        void swapBuffers(void);
        void setClip(const Rect &rect);
        void fillRect(int x, int y, size_t w, size_t h);
        void fillRect(const Point &pos, const Size &size);
        void fillRect(const Rect &rect);
        void drawText(const String &text, int x, int y);
        void drawText(const String &text, const Point &);
        size_t drawWrappedText(const String &text,
                               int x,
                               int y,
                               int w,
                               WrapMode wrapMode);
        size_t drawWrappedText(const String &text,
                               const Point &pos,
                               int w,
                               WrapMode wrapMode);

    private:
        Widget *_widget;
        Font _font;
        Color _color;
        Color _textColor;
        Drawable _drawable;
        bool _doubleBuffered;

        XftDraw *_xftdraw;
        XftColor __textColor;
        XRenderColor __color;
        Picture _xrSolidPicture;        /* Xrender Picture used for solid colors */
    };
}

#endif