#ifndef XGUI_FONTXFT_HPP
#define XGUI_FONTXFT_HPP
#include "Types.hpp"
#include "Xgui_defs.hpp"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <string>


namespace Xgui
{
    class Font
    {
    public:
        Font(const std::string &fontname);
        ~Font(void);
        inline std::string fontname(void) { return _fontname; }
        inline XftFont *xftfont(void) { return _xftfont; }
        inline int ascent(void) { return _xftfont->ascent; }
        inline int descent(void) { return _xftfont->descent; }
        inline int lineHeight(void) const { return _lineHeight; }
        inline void setLineHeight(int lineHeight) { _lineHeight = lineHeight; }
        inline int baseLineHeight(void) const { return _xftfont->height; }
        inline double averageAdvanceWidth(void) const { return _averageAdvanceWidth; }
        Size boundingBox(const std::string &text);
        Size textSize(const String &text);
        Size textSize(const String &text, int w, WrapMode wrapMode);
        size_t numLines(const String &text, int w, WrapMode wrapMode);
        Size textSizeAverage(const String &text);
        Size textSizeDirect(const String &text);
        Rect textExtents(std::string text);

    private:
        std::string _fontname;
        double _averageAdvanceWidth;
        int _lineHeight;
        XftFont *_xftfont;
        XRenderColor _xrcolor;
    };
}

#endif