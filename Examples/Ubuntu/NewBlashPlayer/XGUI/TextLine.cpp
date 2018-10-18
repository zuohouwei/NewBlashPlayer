#include "TextLine.hpp"


namespace Xgui
{
    /*
     * Constructor
     */
    TextLine::TextLine(void)
        : _line()
        , _charsPerLine_cached(0)
        , _linesCache(0)
    {
    }

    /*
     * Overloaded constructor.
     *
     * @line - line of text
     */
    TextLine::TextLine(const String &line)
        : _line(line)
        , _charsPerLine_cached(0)
        , _linesCache(0)
    {
    }

    /*
     * Return subline containing position x,
     * and store positing on the subline back in x
     *
     * @x - pointer to x position
     *
     * Return:
     * subline
     * position on subline stored in `x`
     */
    size_t TextLine::getSublineByPos(size_t *x)
    {
        /* get line (a wrapped chunk) */;
        if (*x > _line.size()) {
            /* out of bounds */
            *x = _wlines[_wlines.size() - 1].second;
            return _wlines.size() - 1;
        }
        size_t line = 0;
        for (; line < _wlines.size(); line++)
        {
            LineChunkInfo pair = _wlines[line];
            size_t end = pair.first + pair.second;
            if (*x >= pair.first && *x <= end) {
                /* x position is in the current line */
                *x -= pair.first;
                break;
            }
        }

        return line;
    }

    /*
     * Wrap the line according to the `wrapMode'
     * and the specified max width
     *
     * @w - maximum width of one line
     * @font - font used
     * @wrapMode - wrap mode used
     */
    size_t TextLine::wrap(int w, Font &font, WrapMode wrapMode)
    {
        _wlines.clear();
        if (!_line.size()) {
            /* empty line, wat wrapping m8? */
            _wlines.insert(_wlines.end(), std::make_pair(0, 0));
            return 1;
        }

        switch (wrapMode)
        {
            case NoWrap:
            {
                /* no wrap is neccesary */
                _wlines.insert(_wlines.end(), std::make_pair(0, _line.size()));
                break;
            }
            case Char:
            {
                /* wrap by character */
                for (size_t i=0; i < _line.size();) {
                    size_t base = i;
                    for (; i < _line.size(); i++) {
                        int width = font.textSize(_line.substr(base, i-base)).w();
                        if (width > w) {
                            /* line break */
                            i--;
                            break;
                        }
                    }
                    _wlines.insert(_wlines.end(), std::make_pair(base, i-base));
                }
                break;
            }
            case Word:
            {
                size_t searchStart = 0,
                       end = 0,
                       endBackup = 0;
                if (_line.find_first_not_of(' ', end) == std::string::npos) {
                    /* line full 'o spaces */
                    _wlines.insert(_wlines.end(), std::make_pair(0, _line.size()));
                }
                for (;;) {
                    /* get next wrapped part */
                    searchStart = _line.find_first_not_of(' ', end);
                    if (searchStart == std::string::npos) {
                        /* the end */
                        break;
                    }
                    end = searchStart;
                    endBackup = end;
                    for (;;) {
                        /* get next word */
                        //size_t endBackup = end;
                        end = _line.find_first_of(' ', end);
                        if (end == std::string::npos) {
                            /* no more spaces */
                            end = _line.size();
                        }
                        int lineWidth = font.textSize(_line.substr(searchStart, end - searchStart)).w();
                        if (lineWidth >= w) {
                            /* the last inserted word can't fit */
                            if (endBackup == searchStart) {
                                /* this word's width is bigger than the max width, fall back to char wrapping */
                                for (;searchStart < end;) {
                                    size_t j=0;
                                    int width = 0;
                                    for (j=searchStart; j<=end; j++) {
                                        width = font.textSize(_line.substr(searchStart, j - searchStart)).w();
                                        if (width >= w) {
                                            /* this char is to be wrapped already */
                                            j--;
                                            break;
                                        }
                                    }
                                    if (j == end && width < w) {
                                        /* this part takes up less space than max width */
                                        end = searchStart;
                                        break;
                                    }
                                    _wlines.insert(_wlines.end(), std::make_pair(searchStart, j - searchStart));
                                    searchStart = j;
                                    //return 0;
                                }
                                break;
                            } else {
                                end = endBackup;
                                _wlines.insert(_wlines.end(), std::make_pair(searchStart, end - searchStart));
                                //return 0;
                                break;
                            }
                        } else {
                            /* we can still add more words (maybe) */
                            endBackup = end;
                            end = _line.find_first_not_of(' ', end);
                            if (end == std::string::npos) {
                                /* we reached the end */
                                _wlines.insert(_wlines.end(), std::make_pair(searchStart, endBackup - searchStart));
                                break;
                            }
                        }
                    }

                    //_wlines.insert(_wlines.end(), std::make_pair(i, end-i));
                }
                break;
            }
        }

        return _wlines.size();
    }
}