#ifndef XGUI_TEXTBUFFER_HPP
#define XGUI_TEXTBUFFER_HPP
#include "Widget.hpp"
#include "Painter.hpp"
#include "FontXft.hpp"
#include "TextLine.hpp"
#include "Xgui_defs.hpp"
#include "TextSelection.hpp"

#include <assert.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>


namespace Xgui
{
    /*
     * TextBuffer class, used to manage text.
     * Used internally by TextView and TextEdit
     */
    class TextBuffer
    {
    public:
        TextBuffer(void);
        ~TextBuffer(void);

        inline size_t getNumLines(void) const { return _lines.size(); }
        void setText(const String &text);
        void clear(void);
        inline void wrap(int w, Font &font, WrapMode wrapMode)
        {
            for (size_t lineIndex=0; lineIndex<_lines.size(); lineIndex++) {
                _lines[lineIndex]->wrap(w, font, wrapMode);
            }
        }
        std::vector<TextLine *> *lines(void) { return &_lines; }
        TextLine *getLine(size_t index) { return _lines[index]; }
        size_t getNumSublinesBetween(size_t line1, size_t line2);
        
        inline void setSelectionStart(const Point &pos) { _selection.setSelectionStart(pos); }
        inline void setSelectionStart(int x, int y) { _selection.setSelectionStart(Point(x, y)); }
        
        inline void setSelectionEnd(const Point &pos) { _selection.setSelectionEnd(pos); }
        inline void setSelectionEnd(int x, int y) { _selection.setSelectionEnd(Point(x, y)); }
        
        inline void getSelection(Point *start, Point *end) { _selection.getSelection(start, end); }
        inline void setSelection(const Point &start, const Point &end) { _selection.setSelection(start, end); }
        String getSelectedText(void);
        
    private:
        Widget *_widget;
        TextSelection _selection;
        std::vector<TextLine *> _lines;
    };
}

#endif