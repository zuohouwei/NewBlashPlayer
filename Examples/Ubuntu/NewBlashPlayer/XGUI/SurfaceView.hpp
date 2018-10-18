//
// Created by parallels on 9/8/18.
//

#ifndef XGUI_SURFACEVIEW_HPP
#define XGUI_SURFACEVIEW_HPP

#include "Widget.hpp"
#include "Types.hpp"
#include "Event.hpp"

#include <string>


namespace Xgui
{
    class SurfaceView : public Widget
    {
    public:
        SurfaceView(Widget *parent, int bg, int x, int y, size_t w, size_t h, Flags flags = NoFlags);
        SurfaceView(Widget *parent, int bg, const Point &pos, const Size &size, Flags flags = NoFlags);
        SurfaceView(Widget *parent, int bg, const Rect &rect, Flags flags = NoFlags);

        ~SurfaceView() {}

    public:

    private:
        int _bg;

        void _init(int bg);

        bool create();
    };

}

#endif //XGUI_SURFACEVIEW_HPP
