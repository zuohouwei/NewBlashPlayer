#ifndef XGUI_XGUI_DEFS_HPP
#define XGUI_XGUI_DEFS_HPP

#include <string>
#include <iostream>
#include <functional.hpp>

#define XGUI_UNUSED(x) (void)x
#define debug(x) std::cout << x << std::endl;


namespace Xgui
{
    typedef std::string String;
    class Widget;
    typedef std::function<void*(Widget *)> Callback;
    
    /*
     * Event types
     */
    enum EventType
    {
        MousePress,
        MouseMove,
        MouseRelease,
        KbdPress,
        KbdRelease,
        Scroll,
        Paint,
    };

    /*
     * Mouse buttons
     */
    enum MouseButton
    {
        Left   = 1,
        Middle = 2,
        Right  = 3,
    };

    /*
     * Key modifiers
     */
    typedef enum
    {
        ModNone    = 0b0000,
        ModShift   = 0b0001,
        ModLock    = 0b0010,
        ModControl = 0b0100,
        ModAlt     = 0b1000,
    } Mod;

    /*
     * Wrapping types used by TextView
     */
    typedef enum
    {
        NoWrap,
        Char,
        Word
    } WrapMode;

    /*
     * General flags for all widgets
     */
    typedef enum
    {
        NoFlags        = 0,
        DoubleBuffered = 1,
    } Flags;

    /*
     * Helper type for managing line chunks in a TextLine
     */
    typedef std::pair<size_t, size_t> LineChunkInfo;
}

#endif