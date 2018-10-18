#include "Painter.hpp"
#include "FontXft.hpp"
#include "Types.hpp"
#include "Widget.hpp"
#include "XApp.hpp"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>

#ifdef _XGUI_USE_XDBE
    #include <X11/extensions/Xdbe.h> /* double buffering support */
#endif /* _XGUI_USE_XDBE */

#include <string>
#include <iostream>
#include <assert.h>
#include <cstring>


namespace Xgui
{
    Painter::Painter(Widget *widget)
        : _widget(widget)
        , _font(widget->font())
        , _drawable(widget->X11Window())
        , _doubleBuffered(widget->doubleBuffered())
    {
        if (_doubleBuffered) {
            /* enable double buffering */
            _drawable = XdbeAllocateBackBufferName(xUIGetApp()->getDisplay(), _widget->X11Window(), XdbeBackground);
        }

        _xftdraw = XftDrawCreate(xUIGetApp()->getDisplay(), _drawable,
            xUIGetApp()->getScreen()->XScreen()->root_visual,
            xUIGetApp()->getScreen()->XScreen()->cmap);

        XRenderPictFormat *pictformat = XRenderFindVisualFormat(xUIGetApp()->getDisplay(),
                                                                DefaultVisual(xUIGetApp()->getDisplay(),
                                                                DefaultScreen(xUIGetApp()->getDisplay())));
        assert(pictformat);
        XRenderPictureAttributes attrs;
        attrs.repeat = 1;
        _xrSolidPicture = XRenderCreatePicture(xUIGetApp()->getDisplay(), _drawable, pictformat, CPRepeat, &attrs);

        setColor(widget->color());
        setTextColor(widget->textColor());
    }

    Painter::~Painter(void)
    {
        swapBuffers();
        if (_doubleBuffered) {
            /* deallocate back buffer */
            if (xUIGetApp()->xdbeEnabled()) {
                /* we used Xdbe back buffer */
                assert(XdbeDeallocateBackBufferName(xUIGetApp()->getDisplay(), _drawable));
            }
        }
        XRenderFreePicture(xUIGetApp()->getDisplay(), _xrSolidPicture);
        XftDrawDestroy(_xftdraw);
    }

    void Painter::setColor(const Color &color)
    {
        __color.red = color.r();
        __color.green = color.g();
        __color.blue = color.b();
        __color.alpha = color.a();
        _color = color;
    }

    void Painter::setTextColor(const Color &color)
    {
        XRenderColor xrcolor;
        xrcolor.red = color.r();
        xrcolor.green = color.g();
        xrcolor.blue = color.b();
        xrcolor.alpha = color.a();
        XftColorAllocValue(xUIGetApp()->getDisplay(),
            xUIGetApp()->getScreen()->XScreen()->root_visual,
            xUIGetApp()->getScreen()->XScreen()->cmap,
            &xrcolor, &__textColor);
        _textColor = color;
    }

    void Painter::clear(void)
    {
        fillRect(Point(), _widget->rect().size());
        //XClearWindow(Xgui::display, _widget->X11Window());
    }

    /*
     * Swap buffers if using double buffering, do nothing otherwise
     */
    void Painter::swapBuffers(void)
    {
        if (_doubleBuffered) {
            if (xUIGetApp()->xdbeEnabled()) {
                XdbeSwapInfo swapInfo;
                swapInfo.swap_window = _widget->X11Window();
                swapInfo.swap_action = XdbeBackground;
                assert(XdbeSwapBuffers(xUIGetApp()->getDisplay(), &swapInfo, 1));
                //XFlush(Xgui::display);
            }
        }
    }

    void Painter::setClip(const Rect &rect)
    {
        /* create a region from a rectangle */
        XRectangle xr = {static_cast<short int>(rect.x()),
                         static_cast<short int>(rect.y()),
                         static_cast<short unsigned int>(rect.w()),
                         static_cast<short unsigned int>(rect.h())};
        Region r = XCreateRegion();
        /* r is now a region suitable for clipping */
        XUnionRectWithRegion(&xr, r, r);

        /* set clipping for Xft (text drawing) */
        XftDrawSetClip(_xftdraw, r);
        /* set clipping for Xrender (general drawing) */
        XRenderSetPictureClipRegion(xUIGetApp()->getDisplay(), _xrSolidPicture, r);
        XDestroyRegion(r);
    }

    void Painter::fillRect(int x, int y, size_t w, size_t h)
    {
        XRenderFillRectangle(xUIGetApp()->getDisplay(), PictOpSrc, _xrSolidPicture, &__color, x, y, w, h);
    }

    /* overloaded function, see void Painter::fillRect(int x, int y, int w, int h) */
    void Painter::fillRect(const Point &pos, const Size &size)
    {
        fillRect(pos.x(), pos.y(), size.w(), size.h());
    }

    /* overloaded function, see void Painter::fillRect(int x, int y, int w, int h) */
    void Painter::fillRect(const Rect &rect)
    {
        fillRect(rect.x(), rect.y(), rect.w(), rect.h());
    }

    void Painter::drawText(const String &text, int x, int y)
    {
        String temp = text;
        size_t index = 0;
        while (true) {
             /* Locate the substring to replace. */
             index = temp.find('\t', index);
             if (index == std::string::npos) break;

             /* Make the replacement. */
             temp.replace(index, 1, "    ");

             /* Advance index forward so the next iteration doesn't pick it up as well. */
             index += 1;
        }
        char *_text = const_cast<char *>(temp.c_str()); /* remove const-ness */
        XftChar8 *xfttext = reinterpret_cast<XftChar8 *>(_text);
        XftDrawStringUtf8(_xftdraw, &__textColor, _font.xftfont(), x, y, xfttext, strlen(_text));
    }

    /* overloaded function, see Painter::drawText(const std::string &text, int x, int y) */
    void Painter::drawText(const String &text, const Point &pos)
    {
        drawText(text, pos.x(), pos.y());
    }

    /*
     * Draw text with specified wrap mode, starting at
     * x and y and fitting withing the specified width
     *
     * Return: height of text in lines.
     */
    size_t Painter::drawWrappedText(const String &text,
                                    int x,
                                    int y,
                                    int w,
                                    WrapMode wrapMode)
    {
        size_t numLines = 0;
        if (!text.size()) {
            /* one empty line */
            return 1;
        }

        switch (wrapMode)
        {
            case NoWrap:
            {
                break;
            }
            case Char:
            {
                for (size_t i=0; i<text.size(); numLines++)
                {
                    size_t base = i;
                    for (;i < text.size(); i++)
                    {
                        int width = _font.textSize(text.substr(base, i-base)).w();
                        if (width > w) {
                            /* line break */
                            i--;
                            break;
                        }
                    }

                    /* now draw it */
                    char *line = const_cast<char *>(text.c_str()) + base; /* remove const-ness */
                    XftChar8 *xftLine = reinterpret_cast<XftChar8 *>(line);
                    XftDrawStringUtf8(_xftdraw, &__textColor, _font.xftfont(), x, y, xftLine, i - base);

                    /* advance one line down */
                    y += _font.lineHeight();
                }
                break;
            }
            case Word:
            {
                break;
            }
        }
        return numLines;
    }

    /* overloaded function */
    size_t Painter::drawWrappedText(const String &text,
                                    const Point &pos,
                                    int w,
                                    WrapMode wrapMode)
    {
        return drawWrappedText(text, pos.x(), pos.y(), w, wrapMode);
    }
}