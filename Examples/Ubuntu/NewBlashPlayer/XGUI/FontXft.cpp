#include "FontXft.hpp"
#include "Types.hpp"
#include "Xgui_defs.hpp"
#include "XApp.hpp"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <string>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <assert.h>


namespace Xgui
{
    Font::Font(const std::string &fontname)
        : _fontname(fontname)
        , _averageAdvanceWidth(0)
        , _lineHeight(0)
    {
        //std::cout << ">>>" << _fontname << std::endl;
        _xftfont = XftFontOpenName(xUIGetApp()->getDisplay(), DefaultScreen(xUIGetApp()->getDisplay()), _fontname.c_str());
        _lineHeight = _xftfont->height;

        /*
         * Xft does not provide average width for fonts,
         * so we have to calculate it manually (for the current charmap only)
         */
        FT_Face ftface = XftLockFace(_xftfont);
        FT_ULong charcode = 0;
        FT_UInt gindex = 0;

        charcode = FT_Get_First_Char(ftface, &gindex);
        double totalPixels = 0;
        unsigned long int totalChars = 0;
        for (;gindex;)
        {
            FT_Error error = FT_Load_Glyph(ftface, gindex, FT_LOAD_DEFAULT);
            if (error) break;
            totalPixels += ftface->glyph->metrics.horiAdvance/64; /* horiAdvance is pixel size multiplied by 64 */
            totalChars++;

            charcode = FT_Get_Next_Char(ftface, charcode, &gindex);
        }
        XftUnlockFace(_xftfont);
        _averageAdvanceWidth = totalPixels/totalChars;
    }

    Font::~Font(void)
    {

    }

    /*
     * Return bounding box required to hold the whole string.
     * Simply a wrapper around textExtents()
     */
    Size Font::boundingBox(const std::string &text)
    {
        Rect rect = textExtents(text);
        return Size(rect.w() - rect.x(), rect.h());
    }

    /*
     * Return size of the whole string in pixels.
     * Simply a wrapper around textExtents()
     */
    Size Font::textSize(const String &text)
    {
        Rect rect = textExtents(text);
        return Size(rect.w() - rect.x(), rect.h());
    }

    Size Font::textSizeAverage(const String &text)
    {
        size_t width = text.size() * _averageAdvanceWidth;
        return Size(width, _lineHeight);
    }

    size_t Font::numLines(const String &text,
                          int w,
                          WrapMode wrapMode)
    {
        size_t lines = 0;
        switch (wrapMode)
        {
            case NoWrap:
            {
                return 1;
            }
            case Char:
            {
                for (size_t i=0; i < text.size();)
                {
                    size_t base = i;
                    for (;i < text.size(); i++, lines++)
                    {
                        //int width = (i - base) * _averageAdvanceWidth;
                        int width = textSize(text.substr(base, i-base)).w();
                        if (width > w) {
                            /* line break */
                            i--;
                            break;
                        }
                    }
                }
                break;
            }
            case Word:
            {
                break;
            }
        }

        return lines;
    }

    /*
     * Get size of the text with given wrap mode
     */
    Size Font::textSize(const String &text,     /* text to get size of */
                        int w,                  /* max width of a line */
                        WrapMode wrapMode       /* wrap mode */
                       )
    {
        size_t y = 0;
        switch (wrapMode)
        {
            case NoWrap:
            {
                break;
            }
            case Char:
            {
                for (size_t i=0; i < text.size();)
                {
                    size_t base = i;
                    for (;i < text.size(); i++)
                    {
                        //int width = (i - base) * _averageAdvanceWidth;
                        int width = textSize(text.substr(base, i-base)).w();
                        if (width > w) {
                            /* line break */
                            i--;
                            break;
                        }
                    }
                    /* advance one line down */
                    y += _lineHeight;
                }
                break;
            }
            case Word:
            {
                break;
            }
        }

        return Size(w, y);
    }

    /*
     * Find out and return text extents
     */
    Rect Font::textExtents(String text)
    {
        XGlyphInfo extents;

        /*size_t tabWidth = 40;

        / * XftTextExtents* does not compute the size of tabs * /
        size_t tabs = std::count(text.begin(), text.end(), '\t');
        text.erase(std::remove(text.begin(), text.end(), '\t'), text.end());*/

        char *_text = const_cast<char *>(text.c_str());
        
        XftTextExtentsUtf8(xUIGetApp()->getDisplay(), _xftfont, reinterpret_cast<XftChar8 *>(_text), text.size(), &extents);
        Rect rect(extents.x, extents.y, extents.xOff /*+ tabs * tabWidth*/, extents.height);

        return rect;
    }
}