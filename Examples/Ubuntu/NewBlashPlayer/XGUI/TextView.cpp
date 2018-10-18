#include "TextView.hpp"
#include <sstream>
#include <iostream>


namespace Xgui
{
    TextView::TextView(Widget *parent, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h, flags)
    {
        _init(flags);
    }

    TextView::TextView(Widget *parent, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size, flags)
    {
        _init(flags);
    }

    TextView::TextView(Widget *parent, const Rect &rect, Flags flags)
        : Widget(parent, rect, flags)
    {
        _init(flags);
    }

    TextView::~TextView(void)
    {
        //_lines.clear();
    }

    size_t TextView::scrollUp(size_t pixels)
    {
        if ((uint)_pixelOffset.y() > pixels) {
            /* we can scroll */
            _pixelOffset.setY(_pixelOffset.y() - pixels);
            redraw();
            return pixels;
        }

        size_t offsety = _pixelOffset.y();
        for (size_t lineIndex=_offset.y(); lineIndex;)
        {
            _offset.setY(_offset.y() - 1);
            TextLine *line = _textBuffer->lines()->at(--lineIndex);
            if (line->text()->size()) {
                offsety += line->getNumSublines() * _lineHeight;//_font.lineHeight();
            } else {
                /* empty line */
                offsety += _lineHeight;
            }

            if (offsety >= pixels) {
                /* done */
                break;
            }
        }

        if (offsety < pixels) {
            /* can't scroll `pixels' pixels */
            _offset.setY(0);
            _pixelOffset.setY(0);

            redraw();
            return offsety;
        } else {
            _pixelOffset.setY(offsety - pixels);

            redraw();
            return pixels;
        }
    }

    /* scroll down max `pixels' pixels */
    size_t TextView::scrollDown(size_t pixels)
    {
        /*
        rhcp
        wizards bakshi
        */
        /* first update pixel offset */
        _pixelOffset.setY(_pixelOffset.y() + pixels);

        /* now recalculate number of skipped lines */
        for (size_t lineIndex=_offset.y(); (lineIndex < _textBuffer->getNumLines()) && _pixelOffset.y(); lineIndex++)
        {
            int height = 0;
            TextLine *line = _textBuffer->lines()->at(lineIndex);
            if (line->text()->size()) {
                height = line->getNumSublines() * _lineHeight;
            } else {
                /* empty line */
                height = _lineHeight;
            }
            
            if (_pixelOffset.y() <= height) {
                /* this line will be visible */
                break;
            }

            _pixelOffset.setY(_pixelOffset.y() - height);
            _offset.setY(_offset.y() + 1);
        }

        redraw();

        return pixels;
    }

    /*
     * Scroll up to specified line.
     *
     * @line - line to scroll to
     */
    void TextView::scrollUpToLine(uint line)
    {
        if (static_cast<int>(line) > _offset.y()) {
            return;
        }
        _offset.setY(line);
        _pixelOffset.setY(0);
        redraw();
    }

    /*
     * Scroll down to specified line.
     *
     * @line - line to scroll to
     */
    void TextView::scrollDownToLine(uint line)
    {
        /* FIXME: scroll is wrong */
        if (static_cast<int>(line) < _lastVisibleLine) {
            return;
        }

        size_t pos = 0;
        for (int lineIndex=_offset.y();
             lineIndex < _lastVisibleLine;
             lineIndex++) {
            pos += _textBuffer->getLine(lineIndex)->getNumSublines() * _lineHeight;
        }
        pos -= _pixelOffset.y(); /* now `pos` contains position of the last visible line */
        
        size_t fullLineHeight = _textBuffer->getLine(_lastVisibleLine)->getNumSublines() * _lineHeight;
        size_t scroll = fullLineHeight - _textRect.h() + pos;

        for (size_t lineIndex=_lastVisibleLine + 1;
             lineIndex<=line;
             lineIndex++) {
            scroll += _textBuffer->getLine(lineIndex)->getNumSublines() * _lineHeight;
        }

        /* so we can see the first subline */
        //scroll += _lineHeight;

        scrollDown(scroll);
    }

    /*
     * Scroll to specified line. This function calls
     * scrollUpToLine or scrollDownToLine internally.
     *
     * @line - line to scroll to
     */
    void TextView::scrollToLine(uint line)
    {
        if (isVisible(line)) {
            return;
        }
        if (static_cast<int>(line) < _offset.y()) {
            scrollUpToLine(line);
        } else {
            scrollDownToLine(line);
        }
    }


    void TextView::setTextBuffer(TextBuffer *textBuffer)
    {
        _textBuffer = textBuffer;
        for (size_t i=0; i<_textBuffer->getNumLines(); i++) {
            _textBuffer->lines()->at(i)->wrap(_textRect.w(), _font, _wrapMode);
        }
    }

    /*
     * Update current cursor position, scrolling as 
     * neccesary and updating selection
     *
     * @column - new column position
     * @row - new row position
     */

    /*
     * Move cursor to specified line and specified column
     *
     * @line - line to move to
     * @column - column to move to
     *
     * Note: the function accepts a normal line index, i.e. not
     * a wrapped line index. @column will be used to find the
     * correct subline.
     */
    void TextView::moveCursor(size_t line, size_t column)
    {
        _cursorPos.setXY(column, line);

        /* now we check if the cursor is visible,
         * if it is not we will scroll up/down until
         * it is.
         */
        TextLine *textline = _textBuffer->getLine(_cursorPos.y());

        if (_cursorPos.y() < _offset.y()) {
            /* cursor is off-bounds on the top */
            scrollUpToLine(_cursorPos.y());
            size_t x = _cursorPos.x();
            size_t subline = textline->getSublineByPos(&x);
            if (subline) scrollDown(_lineHeight * subline);
        } else if (_cursorPos.y() == _offset.y()) {
            /* cursor is on the first visible line */
            size_t x = _cursorPos.x();
            size_t subline = textline->getSublineByPos(&x);
            int pos = subline * _lineHeight - _pixelOffset.y();
            if (pos < 0) {
                /* subline is invisible (or is partially visible) */
                scrollUpToLine(_cursorPos.y());
                scrollDown(subline * _lineHeight);
            }
        } else if (_cursorPos.y() == _lastVisibleLine) {
            /* cursor is on the last visible line */
            size_t x = _cursorPos.x();
            size_t subline = textline->getSublineByPos(&x);
            int pos = (subline + 1) * _lineHeight;
            /*for (int lineIndex=_offset.y();
                 lineIndex < _lastVisibleLine;
                 lineIndex++) {
                pos += _textBuffer->getLine(lineIndex)->getNumSublines() * _lineHeight;
            }*/
            pos += _textBuffer->getNumSublinesBetween(_offset.y(), _lastVisibleLine) * _lineHeight;
            pos -= _pixelOffset.y();
            if (pos > static_cast<int>(_textRect.h())) {
                /* subline is invisible (or is partially visible) */
                scrollDownToLine(_cursorPos.y());
                scrollUp((_textBuffer->getLine(_cursorPos.y())->getNumSublines() - subline - 1) * _lineHeight);
            }
        } else if (_cursorPos.y() > _lastVisibleLine) {
            /* cursor is off-bounds on the bottom */
            size_t x = _cursorPos.x();
            size_t subline = textline->getSublineByPos(&x);
            scrollDownToLine(_cursorPos.y());
            scrollUp((textline->getNumSublines() - subline - 1) * _lineHeight);
        }

        redraw();
    }

    /*
     * If selecting, update current selection and redraw
     *
     * @_redraw - whether to call `redraw()`
     */
    void TextView::updateSelection(const KeyboardEventData &event,
                                   bool _redraw = true)
    {
        if (!(event.mods & ModShift)) {
            /* moved without holding down shift, clean selection */
            _selecting = false;
        }
        if (_selecting) {
            _textBuffer->setSelectionEnd(_cursorPos);
        }
        if (_redraw) redraw();
    }

    inline void TextView::drawCursor(Painter &painter,
                                     int x,
                                     int y)
    {
        /* draw cursor only if visible */
        y -= (_lineHeight - _font.lineHeight())/2;
        if (y + _lineHeight > static_cast<uint>(_textRect.y())) {
            painter.setColor(Color(0xfff, 0xfff, 0xfff));
            if (x == _textRect.x()) x++;
            painter.fillRect(x - 1, y, 1, _lineHeight);
        }
    }

    void TextView::highlightLine(Painter &painter,
                                 size_t lineIndex,
                                 size_t startX,
                                 size_t endX,
                                 size_t offsetY)
    {
        painter.setColor(Color::fromRGB(74, 144, 217));
        TextLine *line = _textBuffer->getLine(lineIndex);
        LineChunkInfo info;

        size_t startSubline = line->getSublineByPos(&startX);
        size_t endSubline = line->getSublineByPos(&endX);
        size_t wExtra = 5;

        size_t height = _lineHeight;//_font.lineHeight();
        offsetY += startSubline * _lineHeight;
        offsetY -= (_lineHeight - _font.lineHeight())/2;

        if (startSubline == endSubline) {
            /* selection is just on one subline */
            String chunk = line->getSubline(startSubline);
            int x = _font.textSize(chunk.substr(0, startX)).w();
            int w = _font.textSize(chunk.substr(0, endX)).w() - x;
            
            if (endX == chunk.size()) {
                w += wExtra;
            }
            painter.fillRect(_textRect.x() + x - 2, _textRect.y() + offsetY, w + 1, height);
            offsetY += _lineHeight;
            return;
        }

        for (size_t subline=startSubline;
             subline < endSubline;
             subline++)
        {
            String chunk = line->getSubline(subline);
            int x = _font.textSize(chunk.substr(0, startX)).w();
            int w = _font.textSize(chunk).w() - x + wExtra;
            painter.fillRect(_textRect.x() + x - 2, _textRect.y() + offsetY, w + 4, height);
            offsetY += _lineHeight;
            startX = 0;
        }

        /* render last subline */
        String chunk = line->getSubline(endSubline);
        int x = 0;
        int w = _font.textSize(chunk.substr(0, endX)).w();
        painter.fillRect(_textRect.x() + x - 2, _textRect.y() + offsetY, w + 1, height);
        offsetY += _lineHeight;
    }

    void TextView::paintEvent(PaintEventData &)
    {
        Painter painter(this);
        /* fill background */
        painter.fillRect(0, 0, _rect.w(), _rect.h());

        painter.setClip(_textRect);
        
        /* draw visible lines */
        switch (_wrapMode)
        {
            case NoWrap:
            {
                /* no line wrapping */
                break;
            }
            case Char:
            {
                /* character wrapping */
                break;
            }
            case Word:
            {
                /* wrap by word */
                uint lineIndex = _offset.y();
                int textHeight = -_pixelOffset.y();
                size_t cursorY = 0;
                Point selStart, selEnd;
                _textBuffer->getSelection(&selStart, &selEnd);

                for (;lineIndex < _textBuffer->getNumLines() && textHeight < static_cast<int>(_textRect.y() + _textRect.h());
                      lineIndex++)
                {
                    if (_selecting && static_cast<int>(lineIndex) >= selStart.y() && static_cast<int>(lineIndex) <= selEnd.y()) {
                        /* highlight current line */
                        size_t startX = 0;
                        size_t endX = _textBuffer->getLine(lineIndex)->text()->size();
                        if (static_cast<int>(lineIndex) == selStart.y()) {
                            /* first selected line */
                            startX = selStart.x();
                        }
                        if (static_cast<int>(lineIndex) == selEnd.y()) {
                            /* last selected line */
                            endX = selEnd.x();
                        }
                        highlightLine(painter, lineIndex, startX, endX, textHeight);
                    }
                    if (lineIndex == static_cast<uint>(_cursorPos.y())) {
                        cursorY = textHeight;
                    }
                    TextLine *line = _textBuffer->lines()->at(lineIndex);
                    for (size_t i=0; i<line->getNumSublines(); i++) {
                        String chunk = line->subline(i);
                        painter.drawText(chunk, _textRect.x(), _textRect.y() + textHeight + _font.ascent());
                        textHeight += _lineHeight;
                    }
                }
                _lastVisibleLine = lineIndex - 1;

                /* render the cursor */
                if (_cursorPos.y() >= _offset.y() && _cursorPos.y() <= _lastVisibleLine) {
                    TextLine *current = _textBuffer->lines()->at(_cursorPos.y());
                    size_t posX = _cursorPos.x();
                    size_t subline = current->lineFromPos(&posX);

                    int x = _textRect.x() + _font.textSize(current->subline(subline).substr(0, posX)).w();
                    
                    drawCursor(painter, x, _textRect.y() + cursorY + (_lineHeight * subline));
                }
                break;
            }
        }
    }

    void TextView::keyPressEvent(KeyboardEventData &event)
    {
        TextLine *line = _textBuffer->getLine(_cursorPos.y());
        switch (event.key)
        {
            case KeyShift_L:
            case KeyShift_R:
            {
                if (!_selecting) {
                    _selecting = true;
                    _textBuffer->setSelectionStart(_cursorPos);
                }
                break;
            }
            case KeyUp:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }
                
                /* move cursor up a line */
                if (_cursorPos.y() == 0) {
                    /* we are on the first line already */
                    break;
                }

                if (line->isMultiline()) {
                    /* move inside current line instead */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    if (!subline) {
                        /* nope, go up a line */
                        line = _textBuffer->getLine(_cursorPos.y() - 1);
                        /* make sure we go to the last subline */
                        LineChunkInfo info = line->getSublineInfo(line->getNumSublines() - 1);
                        moveCursor(_cursorPos.y() - 1, info.first + _cursorPos.x());
                    } else {
                        /* go up a subline */
                        LineChunkInfo info = line->getSublineInfo(subline - 1);
                        if (x > info.second)
                            x = info.second;

                        moveCursor(_cursorPos.y(), info.first + x);
                    }
                } else {
                    /* go one line up */
                    line = _textBuffer->getLine(_cursorPos.y() - 1);
                    /* make sure we go to the last subline */
                    LineChunkInfo info = line->getSublineInfo(line->getNumSublines() - 1);
                    moveCursor(_cursorPos.y() - 1, info.first + _cursorPos.x());
                }
                
                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
            case KeyDown:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }
                
                if (line->isMultiline()) {
                    /* move inside current line instead */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    if (++subline >= line->getNumSublines()) {
                        /* nope, advance to the next line */
                        if (_textBuffer->getNumLines() - 1 == static_cast<uint>(_cursorPos.y())) {
                            /* already on latest line */
                            moveCursor(_cursorPos.y(), line->getSize());
                        } else {
                            moveCursor(_cursorPos.y() + 1, x);
                        }
                    } else {
                        /* advance x */
                        LineChunkInfo info = line->getSublineInfo(subline);
                        if (x > info.second)
                            x = info.second;
                        
                        moveCursor(_cursorPos.y(), info.first + x);
                    }
                } else {
                    /* advance one line down */
                    if (_textBuffer->getNumLines() - 1 == static_cast<uint>(_cursorPos.y())) {
                        /* already on latest line */
                        moveCursor(_cursorPos.y(), line->getSize());
                    } else {
                        moveCursor(_cursorPos.y() + 1, _cursorPos.x());
                    }
                }

                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
            case KeyLeft:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }
                
                if (static_cast<uint>(_cursorPos.x()) > line->getSize())
                    _cursorPos.setX(line->getSize());

                if (event.mods & ModAlt) {
                    /* keyboard shortcut - move to the beginning of the current subline */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    LineChunkInfo info = line->getSublineInfo(subline);
                    moveCursor(_cursorPos.y(), info.first);
                    if (_selecting) {
                        _textBuffer->setSelectionEnd(_cursorPos);
                        redraw();
                    }
                    break;
                }

                /* go left one character */
                if (!_cursorPos.x()) {
                    /* is this the very first line? */
                    if (_cursorPos.y()) {
                        /* no, goto one line up */
                        size_t prevLine = _cursorPos.y() - 1;
                        moveCursor(prevLine, _textBuffer->getLine(prevLine)->getSize());
                    }
                } else {
                    moveCursor(_cursorPos.y(), _cursorPos.x() - 1);
                }

                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
            case KeyRight:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }
                
                if (event.mods & ModAlt) {
                    /* keyboard shortcut - move to the end of the current subline line */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    LineChunkInfo info = line->getSublineInfo(subline);
                    
                    moveCursor(_cursorPos.y(), info.first + info.second);
                    if (_selecting) {
                        _textBuffer->setSelectionEnd(_cursorPos);
                        redraw();
                    }
                    break;
                }

                if (static_cast<uint>(_cursorPos.x()) >= line->getSize()) {
                    /* goto next line */
                    if (static_cast<uint>(_cursorPos.y()) == _textBuffer->getNumLines() - 1) {
                        /* already on latest line */
                        moveCursor(_cursorPos.y(), line->getSize());
                    } else {
                        moveCursor(_cursorPos.y() + 1, 0);
                    }
                } else {
                    moveCursor(_cursorPos.y(), _cursorPos.x() + 1);
                }

                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
            case KeyPage_Down:
            {
                size_t totalNumVisibleLines = _textRect.h()/_lineHeight;
                size_t tmp = _cursorPos.x();
                size_t numSublines = 0;
                size_t lineIndex = _offset.y();
                size_t currentSubline = line->getSublineByPos(&tmp);

                for (; lineIndex < _textBuffer->getNumLines(); lineIndex++)
                {
                    size_t sublines = _textBuffer->getLine(lineIndex)->getNumSublines();
                    if (numSublines + sublines >= totalNumVisibleLines)
                        break;

                    numSublines += sublines;
                }
                if (lineIndex == _textBuffer->getNumLines()) {
                    /* move to the end of the document */
                    size_t lastLine = _textBuffer->getNumLines() - 1;
                    moveCursor(lastLine, _textBuffer->getLine(lastLine)->getSize());
                } else {
                    /* move cursor down */
                    scrollDown((totalNumVisibleLines) * _lineHeight);
                    size_t subline = totalNumVisibleLines - numSublines - 1;
                    size_t x = _cursorPos.x(); (void) line->getSublineByPos(&x);
                    line = _textBuffer->getLine(lineIndex);
                    LineChunkInfo info = line->getSublineInfo(subline);
                    debug("lineIndex: " << lineIndex
                       << ", subline: " << subline
                       << ", totalNumVisibleLines: " << totalNumVisibleLines
                       << ", numSublines: " << numSublines
                       << ", info.first: " << info.first);
                    moveCursor(lineIndex, info.first + x);
                }

                break;
            }
            case KeyHome:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }

                if (event.mods & ModControl) {
                    moveCursor(0, 0);
                } else {
                    /* move to the beginning of the current subline */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    LineChunkInfo info = line->getSublineInfo(subline);
                    moveCursor(_cursorPos.y(), info.first);
                }

                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
            case KeyEnd:
            {
                if (!(event.mods & ModShift)) {
                    /* moved without holding shift down, clean selection */
                    _selecting = false;
                }

                if (event.mods & ModControl) {
                    /* move to the end of the file */
                    line = _textBuffer->getLine(_textBuffer->getNumLines() - 1);
                    moveCursor(_textBuffer->getNumLines() - 1, line->getSize());
                } else {
                    /* move to the end of the current subline */
                    size_t x = _cursorPos.x();
                    size_t subline = line->getSublineByPos(&x);
                    LineChunkInfo info = line->getSublineInfo(subline);
                    moveCursor(_cursorPos.y(), info.first + info.second);
                }

                if (_selecting) {
                    _textBuffer->setSelectionEnd(_cursorPos);
                    redraw();
                }
                break;
            }
        }
    }

    void TextView::keyReleaseEvent(KeyboardEventData &)
    {
    }

    void TextView::scrollEvent(ScrollEventData &event)
    {
        switch (event.direction)
        {
            case ScrollEventData::Up:
                scrollUp(_lineHeight);
                break;
            case ScrollEventData::Down:
                scrollDown(_lineHeight);
                break;
            default:
                /* ignore horizontal scrolling for now */
                break;
        }
    }

    void TextView::_init(Flags)
    {
        const int margin = 10;
        _textRect = Rect(margin, margin, _rect.w() - margin * 2, _rect.h() - margin * 2);
        _lastVisibleLine = 0;
        _wrapMode = Word;
        _lineHeight = _font.lineHeight();
        _selecting = false;

        setColor(Xgui::Color::fromRGB(245, 245, 245));
    }
}
