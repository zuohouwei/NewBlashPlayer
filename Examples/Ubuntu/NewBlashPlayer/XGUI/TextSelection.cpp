#include "TextSelection.hpp"


namespace Xgui
{
    TextSelection::TextSelection(void)
    {

    }

    TextSelection::~TextSelection(void)
    {

    }

    /*
     * Set selection start.
     *
     * @pos - position of selection start.
     */
    void TextSelection::setSelectionStart(const Point &pos)
    {
        _start.copyFrom(pos);
    }

    /*
     * Set selection end.
     *
     * @pos - position of selection end.
     */
    void TextSelection::setSelectionEnd(const Point &pos)
    {
        _end.copyFrom(pos);
    }

    /*
     * Get selection range into two <Point>s
     * 
     * @start - beginning of selection
     * #end - end of selection
     */
    void TextSelection::getSelection(Point *start, Point *end)
    {
        if (_start.y() == _end.y()) {
            /* selection is on the same line */
            if (_start.x() < _end.x()) {
                /* selection is from left to right */
                start->copyFrom(_start);
                end->copyFrom(_end);
            } else {
                /* selection is from right to left */
                start->copyFrom(_end);
                end->copyFrom(_start);
            }
        } else if (_start.y() < _end.y()) {
            /* selection is from top to bottom */
            start->copyFrom(_start);
            end->copyFrom(_end);
        } else {
            /* selection is from bottom to top */
            start->copyFrom(_end);
            end->copyFrom(_start);
        }
    }
}
