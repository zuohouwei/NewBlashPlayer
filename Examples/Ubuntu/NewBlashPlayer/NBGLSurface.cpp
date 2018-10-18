//
// Created by liuenbao on 18-9-15.
//

#include "NBGLSurface.h"

NBGLSurface::NBGLSurface(Widget *parent, int bg, int x, int y, size_t w, size_t h, Flags flags)
    :SurfaceView(parent, bg, x, y, w, h, flags) {

}

NBGLSurface::NBGLSurface(Widget *parent, int bg, const Point &pos, const Size &size, Flags flags)
    :SurfaceView(parent, bg, pos, size, flags) {

}

NBGLSurface::NBGLSurface(Widget *parent, int bg, const Rect &rect, Flags flags)
    :SurfaceView(parent, bg, rect, flags) {

}

NBGLSurface::~NBGLSurface() {

}

void NBGLSurface::keyPressEvent(KeyboardEventData* event)
{
    std::cout << "keyPressEvent in NBGLSurface : " << event->key << std::endl;
}

void NBGLSurface::keyReleaseEvent(KeyboardEventData* event)
{
    std::cout << "keyReleaseEvent in NBGLSurface: " << event->key << std::endl;
}