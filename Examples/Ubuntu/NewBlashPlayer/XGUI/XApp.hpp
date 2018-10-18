//
// Created by liuenbao on 18-9-15.
//

#ifndef XAPP_H
#define XAPP_H

#include <ev.h>
#include <X11/Xlib.h>
#include <unordered_map.hpp>

#include "Widget.hpp"
#include "Window.hpp"
#include "Screen.hpp"
#include "KeyCodes.hpp"

namespace Xgui {

    class Widget;

    class XApp {
    public:
        XApp();

        virtual ~XApp();

    public:
        void addWidget(Widget *widget);

        virtual void onBeforeRun();

        virtual void onRun();

        virtual void onExitRun();

        virtual int initialize(int argc, char *argv[]);

        virtual void terminate();

        Display *getDisplay() {
            return display;
        }

        Screen *getScreen() {
            return screen;
        }

        std::unordered_map<int, Widget*>& getWidgets() {
            return widgets;
        }

        inline void setTopLevelWindow(Window *window) { topLevelWindow = window; }

        inline Window* getTopLevelWindow() { return topLevelWindow; }

        bool xdbeEnabled(void);

    protected:
        Xgui::Window *window;

    private:
        struct ev_loop *loop;
        ev_io display_io;
        Display *display;
        Screen *screen;
        Window *topLevelWindow;
        std::unordered_map<int, Widget*> widgets;
    };

    typedef XApp *(*AllocAppInstanceFunc)();

    typedef void (*FreeAppInstanceFunc)(XApp *app);

    void regAppLifeFunc(AllocAppInstanceFunc allocApp, FreeAppInstanceFunc freeApp);

#define X_APP_IMPLEMENT(appName)                                    \
        static XApp* AllocApp##appName() {                          \
            return new appName;                                     \
        }                                                           \
        static void FreeApp##appName(XApp* app) {                   \
            delete app;                                             \
        };                                                          \
        class XAppLauncher {                                        \
        public:                                                     \
            XAppLauncher() {                                        \
                regAppLifeFunc(AllocApp##appName, FreeApp##appName);\
            }                                                       \
        };                                                          \
        static XAppLauncher g_AppLauncher;                          \


    int xUIAppMainEntry(int argc, char *argv[]);

    XApp *xUIGetApp();
}

#endif //XAPP_H
