#ifndef XGUI_TEXTLINE_HPP
#define XGUI_TEXTLINE_HPP
#include "FontXft.hpp"
#include "Xgui_defs.hpp"

#include <vector>
#include <utility>
#include <iostream>


namespace Xgui
{
    /*
     * Class for representing a single line of text.
     */
    class TextLine
    {
    public:
        TextLine(void);
        TextLine(const String &line);
        inline bool isMultiline(void) const { return _wlines.size() > 1; }
        inline size_t lineFromPos(size_t *x) { return getSublineByPos(x); }
        size_t getSublineByPos(size_t *x);
        LineChunkInfo getSublineInfo(size_t subline) { return _wlines[subline]; }
        inline String subline(size_t subline) { return getSubline(subline); }
        inline String getSubline(size_t subline)
        {
            LineChunkInfo info = _wlines[subline];
            return _line.substr(info.first, info.second);
        }
        inline String *text(void) { return &_line; }
        inline size_t getSize(void) const { return _line.size(); }
        size_t wrap(int w, Font &font, WrapMode wrapMode);
        inline size_t getNumSublines(void) const { return _wlines.size(); }
        inline std::vector<LineChunkInfo> *wrappedLines(void) { return &_wlines; }

    private:
        String _line;
        std::vector<LineChunkInfo> _wlines;
        size_t _charsPerLine_cached;
        size_t _linesCache;
    };
}

#endif