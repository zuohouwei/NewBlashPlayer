#ifndef XGUI_EVENT_HPP
#define XGUI_EVENT_HPP
#include "Types.hpp"
#include "Xgui_defs.hpp"

#include <X11/keysym.h>
#include <string>


namespace Xgui
{
/*
    enum KeyCode
    {
        BackSpace = static_cast<Char>(XK_BackSpace),                / * Back space, back char * /
        Tab = static_cast<Char>(XK_Tab),
        Linefeed = static_cast<Char>(XK_Linefeed),                  / * Linefeed, LF * /
        Clear = static_cast<Char>(XK_Clear),
        Return = static_cast<Char>(XK_Return),                      / * Return, enter * /
        Pause = static_cast<Char>(XK_Pause),                        / * Pause, hold * /
        Scroll_Lock = static_cast<Char>(XK_Scroll_Lock),
        Sys_Req = static_cast<Char>(XK_Sys_Req),
        Escape = static_cast<Char>(XK_Escape),
        Delete = static_cast<Char>(XK_Delete),                      / * Delete, rubout * /
    };
*/

    /* mouse event data */
    typedef struct
    {
        Point pos;
        MouseButton button;
    } MouseEventData;

    /* keyboard event data */
    typedef struct
    {
        size_t key;
        size_t mods;
    } KeyboardEventData;

    /* scroll event data */
    typedef struct
    {
        enum Direction
        {
            Up,
            Down,
            Left,
            Right
        } direction;
    } ScrollEventData;

    /* paint event data */
    typedef struct
    {
        /* to be filled */
        void *data;
    } PaintEventData;

    /* event structure */
    typedef struct
    {
        EventType type;
        union
        {
            MouseEventData* mousedata;
            PaintEventData* paintdata;
            KeyboardEventData* keyboarddata;
            ScrollEventData* scrolldata;
            void *customdata;
        };
    } Event;
}

#endif