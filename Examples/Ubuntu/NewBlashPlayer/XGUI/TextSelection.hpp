#ifndef XGUI_TEXTSELECTION_CPP
#define XGUI_TEXTSELECTION_CPP
#include "Types.hpp"


namespace Xgui
{
    /*
     * Class for managing text selection, used internally by TextBuffer class
     */
    class TextSelection
    {
    public:
        TextSelection(void);
        ~TextSelection(void);

        inline void setSelection(const Point &start, const Point &end) { _start.copyFrom(start); _end.copyFrom(end); }
        inline void clearSelection(void) { _start.copyFrom(Point()); _end.copyFrom(Point()); }
        void setSelectionStart(const Point &pos);
        inline Point getSelectionStart(void) const { return _start; }
        void setSelectionEnd(const Point &pos);
        inline Point getSelectionEnd(void) const { return _end; }
        void getSelection(Point *start, Point *end);
    
    protected:
        Point _start;
        Point _end;
    };
}

#endif