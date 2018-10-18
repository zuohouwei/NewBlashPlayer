#ifndef XGUI_INPUT_HPP
#define XGUI_INPUT_HPP
#include "Widget.hpp"
#include "Event.hpp"
#include "Xgui_defs.hpp"


namespace Xgui
{
    class Input : public Widget
    {
    public:
        Input(Widget *parent, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        Input(Widget *parent, const Point &pos, const Size &size, Flags flags = NoFlags);
        Input(Widget *parent, const Rect &rect, Flags flags = NoFlags);
        ~Input(void);
        inline size_t caretPos(void) const { return _caretPos; }
        inline Color selectionColor(void) const { return _selectionColor; }
        inline void setSelectionColor(const Color &color) { _selectionColor = color; }
        std::string value(void) const { return _text; }
        void removeText(size_t start, size_t length);
        void selectionRange(size_t *start, size_t *end);

        size_t numVisibleChars(void);

    protected:
        virtual void paintEvent(PaintEventData &);
        virtual void keyPressEvent(KeyboardEventData &);
        virtual void keyReleaseEvent(KeyboardEventData &);
    
    private:
        std::string _text;
        Rect _textRect;
        Color _selectionColor;
        size_t _caretPos;
        size_t _charOffset;
        size_t _pixelOffset;
        bool _selecting;
        bool _selected;
        int _ctrlDown;
        size_t _selectionStart;
        size_t _selectionEnd;

        void _init(Flags flags);
        void _updateOffsets(void);
    };
}

#endif