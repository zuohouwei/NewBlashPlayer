#include "Input.hpp"
#include "KeyCodes.hpp"

#include <iostream>


namespace Xgui
{
    Input::Input(Widget *parent, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h, flags)
        , _selectionColor(Color::fromRGB(74, 144, 217))
        , _caretPos(0)
        , _charOffset(0)
        , _pixelOffset(0)
        , _selecting(false)
        , _selected(false)
        , _ctrlDown(0)
        , _selectionStart(0)
        , _selectionEnd(0)
    {
        _init(flags);
    }

    Input::Input(Widget *parent, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size, flags)
        , _selectionColor(Color::fromRGB(74, 144, 217))
        , _caretPos(0)
        , _charOffset(0)
        , _pixelOffset(0)
        , _selecting(false)
        , _selected(false)
        , _ctrlDown(0)
        , _selectionStart(0)
        , _selectionEnd(0)
    {
        _init(flags);
    }

    Input::Input(Widget *parent, const Rect &rect, Flags flags)
        : Widget(parent, rect, flags)
        , _selectionColor(Color::fromRGB(74, 144, 217))
        , _caretPos(0)
        , _charOffset(0)
        , _pixelOffset(0)
        , _selecting(false)
        , _selected(false)
        , _ctrlDown(0)
        , _selectionStart(0)
        , _selectionEnd(0)
    {
        _init(flags);
    }

    Input::~Input(void)
    {
    }

    /*
     * Obtain the amount of currently visible characters.
     */
    size_t Input::numVisibleChars(void)
    {
        /* full width of the string to be rendered */
        size_t realWidth = _textRect.w() + _pixelOffset;

        /* minimum amout of visible characters */
        size_t chars = 0;//realWidth/_font.xftfont()->max_advance_width;
        if (chars >= _text.size()) {
            /* all characters can are visible, guaranteed */
            std::cout << "all visible: " << chars << " " << _text.size() << std::endl;
            return _text.size();
        }

        size_t max = _text.size() - _charOffset;
        for (;chars < max; chars++) {
            size_t width = _font.boundingBox(_text.substr(_charOffset, chars)).w();
            if (width >= realWidth) {
                /* this character won't be visible */
                break;
            }
        }

        return chars;
    }

    void Input::_updateOffsets(void)
    {
        if (_caretPos == _text.size()) {
            /* align text on the right */
            std::string temp = _text.substr(_charOffset, _caretPos - _charOffset);
            size_t width = _font.boundingBox(temp).w();
            if (width >= _textRect.w()) {
                _pixelOffset = width - _textRect.w();
            } else {
                _pixelOffset = 0;
            }
        }
        if (_caretPos && _caretPos - 1 > _charOffset) {
            /* caret moved right */
            size_t length = _caretPos - _charOffset + 1;
            if (length + _charOffset > _text.size()) {
                length = _text.size() - _charOffset;
            }
            std::string temp = _text.substr(_charOffset, length);
            size_t caretPixelPos = _font.boundingBox(temp).w();
            if (caretPixelPos - _pixelOffset > _textRect.w()) {
                /* must scroll */
                _pixelOffset = caretPixelPos - _textRect.w();
            }

            /* update character offset */
            std::string hiddenText(1, _text[_charOffset]);
            size_t width = 0;
            for (;;) {
                size_t newWidth = _font.boundingBox(hiddenText).w();
                if (newWidth > _pixelOffset) {
                    /* this character will be visible */
                    break;
                }

                width = newWidth;
                hiddenText += _text[++_charOffset];
            }
            _pixelOffset -= width;
        } else {
            /* caret moved left */
            if (_pixelOffset) {
                /* remove pixel offset, if any */
                _pixelOffset = 0;
            } else if (_charOffset) {
                /* move left one char */
                if (_caretPos) {
                    _charOffset = _caretPos - 1;
                } else {
                    _charOffset = 0;
                }
            }
        }
    }

    void Input::paintEvent(PaintEventData &)
    {
        Painter painter(this);
        painter.clear();
        
        size_t visibleChars = numVisibleChars();
        std::string temp = _text.substr(_charOffset, visibleChars);
        size_t caretPixelPos = _font.boundingBox(temp.substr(0, _caretPos - _charOffset)).w();

        std::cout << "``" << temp << "`` - " << temp.size() << std::endl;

        if (_selected) {
            painter.setColor(_selectionColor);//Color::fromRGB(74, 144, 217));
            /* some text is selected */
            if (_selectionStart < _selectionEnd) {
                /* selection from left to right */
                int start_x = _textRect.x();
                size_t width = 0;
                if (_selectionStart <= _charOffset) {
                    /* part of selection is off-screen on the left */
                    width = _font.boundingBox(temp.substr(0, _selectionEnd - _charOffset)).w() - _pixelOffset;
                } else {
                    /* all of the selection is visible */
                    start_x += _font.boundingBox(temp.substr(0, _selectionStart - _charOffset)).w() - _pixelOffset;
                    width = _font.boundingBox(temp.substr(_selectionStart - _charOffset, _selectionEnd - _selectionStart)).w();
                }
                Rect r(start_x,
                       _textRect.y(),
                       width,
                       _textRect.h());
                painter.fillRect(r);
            } else if (_selectionStart > _selectionEnd) {
                /* selection from right to left */
                int start_x = 0;
                size_t width = 0;
                size_t caretPixelPos = _font.boundingBox(temp.substr(0, _selectionEnd - _charOffset)).w() - _pixelOffset;
                start_x = _textRect.x() + caretPixelPos;
                if (_selectionStart > _charOffset + numVisibleChars()) {
                    /* part of selection is off-screen on the right */
                    width = _textRect.w() - caretPixelPos;
                } else {
                    /* all of the selection is visible */
                    width = _font.boundingBox(temp.substr(0, _selectionStart - _charOffset)).w() - caretPixelPos - _pixelOffset;
                }
                Rect r(start_x,
                       _textRect.y(),
                       width,
                       _textRect.h());
                painter.fillRect(r);
            }
        }

        /* render text */
        int y = (_textRect.h() - _font.lineHeight())/2;
        painter.setClip(_textRect);
        painter.drawText(temp, _textRect.x() - _pixelOffset, _textRect.y() + y + _font.ascent());

        /* render caret */
        painter.setColor(Color(0, 0, 0));
        painter.fillRect(_textRect.x() + caretPixelPos - _pixelOffset, _textRect.y(), 1, _textRect.h());
    }

    void Input::removeText(size_t start, size_t length)
    {
        _text.erase(start, length);
        _selected = false;
        _caretPos = start;

        if (_text.size() <= _charOffset) {
            /* all characters are now visible */
            _charOffset = 0;
            _pixelOffset = 0;
            return;
        }
        
        /* recalculate text positioning */
        size_t visibleChars = numVisibleChars();
        if (_charOffset + visibleChars == _text.size()) {
            /* align text on the right */
            std::string temp = _text.substr(_charOffset, visibleChars);
            size_t width = 0;
            for (;_charOffset;) {
                width = _font.boundingBox(temp).w();
                if (width >= _textRect.w()) {
                    /* last char to be visible */
                    break;
                }

                temp.insert(0, 1, _text[--_charOffset]);
            }
            if (width >= _textRect.w()) {
                _pixelOffset = width - _textRect.w();
            } else {
                /* all characters are visible without any offsets */
                _pixelOffset = 0;
            }
        }
    }

    void Input::selectionRange(size_t *start, size_t *end)
    {
        if (_selected) {
            if (_selectionStart > _selectionEnd) {
                *start = _selectionEnd;
                *end = _selectionStart;
            } else {
                *start = _selectionStart;
                *end = _selectionEnd;
            }
        } else {
            *start = _caretPos;
            *end = _caretPos;
        }
    }

    void Input::keyPressEvent(KeyboardEventData &event)
    {
        switch (event.key) {
            case KeyBackSpace:
            {
                /* remove char before caret */
                if (_selected && _selectionStart != _selectionEnd) {
                    /* remove selection */
                    size_t start = 0, nchars = 0;
                    if (_selectionStart > _selectionEnd) {
                        start = _selectionEnd;
                        nchars = _selectionStart - _selectionEnd;
                    } else {
                        start = _selectionStart;
                        nchars = _selectionEnd - _selectionStart;
                    }
                    removeText(start, nchars);
                    break;
                }

                if (_caretPos) {
                    removeText(_caretPos - 1, 1);
                    break;
                }
                break;
            }
            case KeyDelete:
            {
                /* remove char after caret */
                if (_selected && _selectionStart != _selectionEnd) {
                    /* remove selection */
                    size_t start = 0, nchars = 0;
                    if (_selectionStart > _selectionEnd) {
                        start = _selectionEnd;
                        nchars = _selectionStart - _selectionEnd;
                    } else {
                        start = _selectionStart;
                        nchars = _selectionEnd - _selectionStart;
                    }
                    removeText(start, nchars);
                    break;
                }

                if (_caretPos) {
                    removeText(_caretPos, 1);
                    break;
                }
                break;
            }
            case KeyLeft:
            {
                /* move caret left */
                if (_caretPos) {
                    if (_ctrlDown) {
                        size_t newPos = _caretPos;
                        /* skip left until first non delimiter character */
                        newPos = _text.find_last_not_of(" )]},.", _caretPos - 1);
                        if (newPos > _text.size()) {
                            newPos = _text.size();
                        }
                        /* skip left until the first delimiter character */
                        newPos = _text.find_last_of(" )]},.", newPos) + 1;
                        if (newPos > _text.size()) {
                            newPos = _text.size();
                        }
                        _caretPos = newPos;
                    } else {
                        _caretPos--;
                    }
                }
                if (_selecting) { _selectionEnd = _caretPos; _selected = true; }
                else _selected = false;
                _updateOffsets();
                break;
            }
            case KeyRight:
            {
                /* move caret right */
                if (_caretPos < _text.size()) {
                    if (_ctrlDown) {
                        size_t newPos = _caretPos;
                        /* skip right until first non delimiter character */
                        newPos = _text.find_first_not_of(" )]},.", _caretPos);
                        if (newPos > _text.size()) {
                            newPos = _text.size();
                        }

                        /* skip right until the first delimiter character */
                        newPos = _text.find_first_of(" )]},.", newPos);
                        if (newPos > _text.size()) {
                            newPos = _text.size();
                        }
                        _caretPos = newPos;
                    } else {
                        _caretPos++;
                    }
                }
                if (_selecting) { _selectionEnd = _caretPos; _selected = true; }
                else _selected = false;
                _updateOffsets();
                break;
            }
            case KeyShift_L:
            {
                _selecting = true;
                if (!_selected) {
                    _selectionStart = _caretPos;
                    _selectionEnd = _caretPos;
                }
                break;
            }
            case KeyShift_R:
                break;
            case KeyControl_L:
            case KeyControl_R:
            {
                _ctrlDown++;
                break;
            }
            default:
            {
                /* write the character */
                std::cout << (size_t) event.key << "__z" << std::endl;
                if (_ctrlDown) {
                    if (event.key == 'a') {
                        /* select all */
                        _selected = true;
                        _selectionStart = 0;
                        _selectionEnd = _text.size();
                        _caretPos = _selectionEnd;
                        break;
                    }
                }
                if (_selected) {
                    /* remove the selection */
                    size_t start = 0;
                    size_t end = 0;
                    selectionRange(&start, &end);
                    //std::cout << start << ",,," << end << std::endl;
                    removeText(start, end - start);
                }
                _text.insert(_caretPos++, 1, event.key);
                _updateOffsets();
                break;
            }
        }

        redraw();
    }

    void Input::keyReleaseEvent(KeyboardEventData &event)
    {
        switch (event.key) {
            case KeyShift_L:
            {
                std::cout << "release" << std::endl;
                _selecting = false;
            }
            case KeyControl_L:
            case KeyControl_R:
            {
                if (_ctrlDown) _ctrlDown--;
            }
        }
        redraw();
    }

    void Input::_init(Flags)
    {
        setFont(Font("Arial-13"));
        int offsety = (_rect.h() - _font.lineHeight())/2;
        _textRect = Rect(5, offsety, _rect.w() - 5 * 2, _rect.h() - offsety * 2);
        setColor(Xgui::Color::fromRGB(255, 255, 255));
    }
}