#include "TextBuffer.hpp"
#include "Widget.hpp"
#include "Painter.hpp"


namespace Xgui
{
    TextBuffer::TextBuffer(void)
        : _selection()
    {

    }

    void TextBuffer::clear(void)
    {
        for (size_t i=0; i<_lines.size(); i++) {
            delete _lines[i];
        }

        _lines.clear();
    }

    void TextBuffer::setText(const std::string &text)
    {
        std::stringstream ss(text);
        clear();

        for (String line; std::getline(ss, line, '\n');) {
            _lines.push_back(new TextLine(line));
        }
    }

    size_t TextBuffer::getNumSublinesBetween(size_t line1, size_t line2)
    {
        size_t numSublines = 0;

        if (line2 >= _lines.size())
            line2 = _lines.size() - 1;

        for (; line1<line2; line1++) {
            numSublines += _lines[line1]->getNumSublines();
        }

        return numSublines;
    }

    String TextBuffer::getSelectedText(void)
    {
        Point start, end;
        _selection.getSelection(&start, &end);
        TextLine *line = _lines[start.y()];
        if (start.y() == end.y()) {
            /* selection on the same line */
            return line->text()->substr(start.x(), end.x() - start.x());
        }

        String text = line->text()->substr(start.x(),
            line->text()->size() - start.x()) + '\n';

        /* put together the pieces */
        for (int lineIndex = start.y() + 1;
             lineIndex < end.y();
             lineIndex++)
        {
            text += *(_lines[lineIndex]->text()) + '\n';
        }

        text += _lines[end.y()]->text()->substr(0, end.x());

        return text;
    }
}