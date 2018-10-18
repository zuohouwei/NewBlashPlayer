//
// Created by liuenbao on 18-9-15.
//

#include "XApp.hpp"
#include "Screen.hpp"
#include "Widget.hpp"
#include "XApp.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#ifdef _XGUI_USE_XDBE
#include <X11/extensions/Xdbe.h>
#endif

namespace Xgui {

    static AllocAppInstanceFunc allocAppInstance = NULL;
    static FreeAppInstanceFunc freeAppInstance = NULL;
    static XApp *gAppInstance = NULL;

    void regAppLifeFunc(AllocAppInstanceFunc allocApp, FreeAppInstanceFunc freeApp) {
        allocAppInstance = allocApp;
        freeAppInstance = freeApp;
    }

    XApp::XApp() {

    }

    XApp::~XApp() {

    }

    void XApp::onBeforeRun() {
        if (window != NULL)
            window->show();
    }

    void XApp::onRun() {
        ev_run(loop);
    }

    void XApp::onExitRun() {
        if (window != NULL)
            window->hide();
    }

    static void display_io_cb(EV_P_ ev_io *w, int revents) {
        /* get next event */
        XEvent event;
        int numEvent = XEventsQueued(xUIGetApp()->getDisplay(), QueuedAfterFlush);

        //if not have event queued
        if (numEvent == 0) {
            return;
        }

        while (numEvent--) {
            XNextEvent(xUIGetApp()->getDisplay(), &event);

            std::unordered_map<int, Widget *>::const_iterator cit = xUIGetApp()->getWidgets().find(event.xany.window);
            if (cit == xUIGetApp()->getWidgets().end()) {
                continue;
            }

            Widget *widget = cit->second;

            /* handle event */
            switch (event.type) {
                /*case KeymapNotify:
                {
                    / *
                     * Keyboard mapping was changed
                     * /
                    //std::cout << "mapping";
                    if (1 || event.xmapping.request == MappingKeyboard) {
                        XRefreshKeyboardMapping(&event.xmapping);
                        //std::cout << ".keyboard";
                    }
                    //std::cout << std::endl;
                    break;
                }*/
                case DestroyNotify: {
                    /*
                     * Some window was destroyed
                     * this is temporary handling code and it is pretty bad
                     * as it only checks if destroyed window was a toplevel one
                     */
                    std::cout << "damn" << std::endl;
                    if (widget == xUIGetApp()->getTopLevelWindow()) {
                        std::cout << "toplevel" << std::endl;
                        ev_break(loop, EVBREAK_ALL);
                    }
                    break;
                }
                case ClientMessage: {
                    /*
                     * Message received from somewhere else.
                     * For now we only check if it is a WM_DELETE_WINDOW
                     */
                    Atom WM_DELETE_WINDOW = XInternAtom(xUIGetApp()->getDisplay(), "WM_DELETE_WINDOW", False);
                    if (static_cast<Atom>(event.xclient.data.l[0]) == WM_DELETE_WINDOW) {
                        ev_break(loop, EVBREAK_ALL);
                        break;
                    }
                    break;
                }
                case Expose: {
                    /*
                     * A window that covered part of our window has moved away, exposing part (or all) of our window.
                     * Our window was raised above other windows.
                     * Our window mapped for the first time.
                     * Our window was de-iconified.
                     */
                    if (event.xexpose.count > 0) {
                        /* there are more expose events to process,
                         * we can safely ignore this one
                         */
                        break;
                    }
                    PaintEventData data = {0};
                    Event ev = {Paint, {.paintdata=&data}};
                    widget->event(&ev);
                    break;
                }
                case ButtonPress: {
                    /*
                     * Mouse button pressed.
                     */
                    if (event.xbutton.button < 4) {
                        /* send event only of a button was pressed,
                         * buttons 4+ are scroll events, see `case ButtonRelease' */
                        MouseEventData data = {Point(event.xbutton.x, event.xbutton.y),
                                               static_cast<Xgui::MouseButton>(event.xbutton.button)};
                        Event ev = {MousePress, {.mousedata=&data}};
                        widget->event(&ev);
                    }
                    break;
                }
                case ButtonRelease: {
                    /*
                     * Mouse button released.
                     */
                    if (event.xbutton.button < 4) {
                        /* normal button release event */
                        MouseEventData data = {Point(event.xbutton.x, event.xbutton.y),
                                               static_cast<Xgui::MouseButton>(event.xbutton.button)};
                        Event ev = {MouseRelease, {.mousedata=&data}};
                        widget->event(&ev);
                    } else {
                        /* scroll event */
                        ScrollEventData::Direction direction = ScrollEventData::Up;
                        switch (event.xbutton.button) {
                            case 4: /* up */
                                direction = ScrollEventData::Up;
                                break;
                            case 5: /* down */
                                direction = ScrollEventData::Down;
                                break;
                            case 6: /* left */
                                direction = ScrollEventData::Left;
                                break;
                            case 7: /* right */
                                direction = ScrollEventData::Right;
                                break;
                        }
                        ScrollEventData data = {direction};
                        Event ev = {Scroll, {.scrolldata=&data}};
                        widget->event(&ev);
                    }
                    break;
                }
                case KeyPress: {
                    KeySym keysym;
                    char string[256] = {0};
                    //keysym = XLookupKeysym(&event.xkey, 3);
                    XLookupString(&event.xkey, string, 256, &keysym, NULL);
                    if (keysym == NoSymbol) {
                        //std::cout << "[press] Key has no symbol: 0x" << std::setbase(16) << keysym << std::endl;
                    } else {
                        char buf[256] = {0};
                        int overflow = 0;
                        XkbTranslateKeySym(xUIGetApp()->getDisplay(), &keysym, 0, buf, 256, &overflow);
                        assert(!overflow); /* can't imagine a single character that will take more than 256 bytes */

                        //std::cout << "[press] 0x" << std::setbase(16) << keysym << " (" << buf << ")" << std::endl;
                        KeyboardEventData data = {keysym, 0};
                        if (event.xkey.state & ShiftMask) data.mods |= ModShift;
                        if (event.xkey.state & LockMask) data.mods |= ModLock;
                        if (event.xkey.state & ControlMask) data.mods |= ModControl;
                        if (event.xkey.state & Mod1Mask) data.mods |= ModAlt;
                        Event ev = {KbdPress, {.keyboarddata=&data}};
                        widget->event(&ev);
                    }
                    break;
                }
                case KeyRelease: {
                    KeySym keysym;
                    char string[256] = {0};
                    //keysym = XLookupKeysym(&event.xkey, 3);
                    XLookupString(&event.xkey, string, 256, &keysym, NULL);
                    if (keysym == NoSymbol) {
                        //std::cout << "[release] Key has no symbol: 0x" << std::setbase(16) << keysym << std::endl;
                    } else {
                        char buf[256] = {0};
                        int overflow = 0;
                        XkbTranslateKeySym(xUIGetApp()->getDisplay(), &keysym, 0, buf, 256, &overflow);
                        assert(!overflow); /* can't imagine a single character that will take more than 256 bytes */

                        //std::cout << "[release] 0x" << std::setbase(16) << keysym << " (" << buf << ")" << std::endl;
                        KeyboardEventData data = {keysym, 0};
                        if (event.xkey.state & ShiftMask) data.mods |= ModShift;
                        if (event.xkey.state & LockMask) data.mods |= ModLock;
                        if (event.xkey.state & ControlMask) data.mods |= ModControl;
                        if (event.xkey.state & Mod1Mask) data.mods |= ModAlt;
                        Event ev = {KbdRelease, {.keyboarddata=&data}};
                        widget->event(&ev);
                    }
                    break;
                }
            }
        }
    }

    int XApp::initialize(int argc, char *argv[]) {
        //allow to background render thread
        XInitThreads();

        display = XOpenDisplay(NULL);
        if (!display) {
            return false;
        }

        screen = new Screen(display);

        loop = ev_loop_new();

        ev_io_init (&display_io, display_io_cb, ConnectionNumber(display), EV_READ);
        ev_io_start(loop, &display_io);

        return 0;
    }

    void XApp::terminate() {
        if (screen != NULL) {
            delete screen;
            screen = NULL;
        }

        ev_io_stop(loop, &display_io);

        ev_loop_destroy(loop);

        XCloseDisplay(display);
    }

    void XApp::addWidget(Widget *widget) {
        widgets[widget->X11Window()] = widget;
    }

    bool XApp::xdbeEnabled(void) {
#ifdef _XGUI_USE_XDBE
        static bool enabled = false;
        static bool tried = false;
        if (!tried) {
            tried = true;
            int major_version_return, minor_version_return;
            if (!XdbeQueryExtension(display, &major_version_return, &minor_version_return)) {
                /* the server does not support the Xdbe extension */
                return false;
            }

            Drawable root = RootWindow(display, DefaultScreen(display));
            int num_screens = 1;

            /* iterate over all visuals supported by Xdbe and see if our visual is supported */
            XdbeScreenVisualInfo *info = XdbeGetVisualInfo(display, &root, &num_screens);
            if (!info) {
                /* an error occured */
                return false;
            }

            for (int i = 0; i < info->count; i++) {
                if (info->visinfo[i].visual == DefaultVisual(display, DefaultScreen(display))->visualid) {
                    /* Xdbe supports our screen visual */
                    enabled = true;
                    return true;
                }
            }
        }

        return enabled;
#else /* compiling without Xdbe */
        return false;
#endif /* _XGUI_USE_XDBE */
    }

    int xUIAppMainEntry(int argc, char *argv[]) {
        int err = 0;

        if (allocAppInstance == NULL) {
            return -1;
        }

        gAppInstance = allocAppInstance();

        assert(xUIGetApp()->initialize(argc, argv) == 0);

//        Xgui::SurfaceView *surfaceView = new Xgui::SurfaceView(window, 0, 0, 0, window->rect().w(), window->rect().h());

        XFlush(xUIGetApp()->getDisplay());

        xUIGetApp()->onBeforeRun();

        XFlush(xUIGetApp()->getDisplay());

        xUIGetApp()->onRun();

        xUIGetApp()->onExitRun();

        XFlush(xUIGetApp()->getDisplay());

        xUIGetApp()->terminate();

        if (freeAppInstance != NULL) {
            freeAppInstance(gAppInstance);
        }
        return 0;
    }

    XApp *xUIGetApp() {
        return gAppInstance;
    }

}