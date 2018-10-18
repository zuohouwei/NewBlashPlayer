#ifndef XGUI_TEXTVIEW_HPP
#define XGUI_TEXTVIEW_HPP
#include "Widget.hpp"
#include "TextBuffer.hpp"
#include "Types.hpp"
#include "Event.hpp"
#include "KeyCodes.hpp"
#include "Xgui_defs.hpp"


namespace Xgui
{
    class TextView : public Widget
    {
    public:
        TextView(Widget *parent, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        TextView(Widget *parent, const Point &pos, const Size &size, Flags flags = NoFlags);
        TextView(Widget *parent, const Rect &rect, Flags flags = NoFlags);
        ~TextView(void);

        size_t scrollUp(size_t pixels);
        size_t scrollDown(size_t pixels);
        void scrollUpToLine(uint line);
        void scrollDownToLine(uint line);
        void scrollToLine(uint line);
        inline size_t lineHeight(void) const { return _lineHeight; }
        inline void setLineHeight(size_t lineHeight) { _lineHeight = lineHeight; }
        inline TextBuffer *textBuffer(void) { return _textBuffer; }
        void setTextBuffer(TextBuffer *textBuffer);
        inline WrapMode wrapMode(void) const { return _wrapMode; }
        inline void setWrapMode(WrapMode wrapMode) 
        {
            _wrapMode = wrapMode;
            /* recalculate wrap for all the lines */
            _textBuffer->wrap(_textRect.w(), _font, _wrapMode);
        }
        void moveCursor(size_t line, size_t column);

    protected:
        Rect _textRect;             /*
                                     * rect containing position and size of the actual area containing text.
                                     * can be used to set margins
                                     */

        Point _offset;              /* offset on x and y in chars and lines correspondingly */
        Point _pixelOffset;         /* offset on x and y in pixels */
        int _lastVisibleLine;       /* self desciptive */
        TextBuffer *_textBuffer;    /* text buffer used for storing and managing text */
        size_t _lineHeight;         /* height of a line in pixels */
        Point _cursorPos;           /* cursor position (column, line) */
        bool _selecting;            /* a flag to indicate whether text is currently being selected */

        WrapMode _wrapMode;         /* wrap type, see enum WrapMode */

        virtual void paintEvent(PaintEventData &);
        virtual void keyPressEvent(KeyboardEventData &event);
        virtual void keyReleaseEvent(KeyboardEventData &event);
        virtual void scrollEvent(ScrollEventData &event);
        inline bool isVisible(int line) const { return line >= _offset.y() && line <= _lastVisibleLine; }

    private:
        void updateSelection(const KeyboardEventData &event, bool _redraw);
        void drawCursor(Painter &painter, int x, int y);
        void highlightLine(Painter &painter, size_t lineIndex, size_t startX, size_t endX, size_t offsetY);
        void _init(Flags flags);
    };
}

#endif