//
// Created by liuenbao on 18-9-15.
//

#ifndef NBGLSURFACE_H
#define NBGLSURFACE_H

#include <Xgui>

using namespace Xgui;

class NBGLSurface : public SurfaceView {
public:
    NBGLSurface(Widget *parent, int bg, int x, int y, size_t w, size_t h, Flags flags = NoFlags);

    NBGLSurface(Widget *parent, int bg, const Point &pos, const Size &size, Flags flags = NoFlags);

    NBGLSurface(Widget *parent, int bg, const Rect &rect, Flags flags = NoFlags);

    ~NBGLSurface();

public:
    virtual void keyPressEvent(KeyboardEventData* event);
    virtual void keyReleaseEvent(KeyboardEventData* event);
};

#endif //NBGLSURFACE_H
