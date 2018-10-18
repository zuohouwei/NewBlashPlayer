//
// Created by liuenbao on 18-9-15.
//

#ifndef NBMAINWINDOW_H
#define NBMAINWINDOW_H

#include <Xgui>
#include <NBMediaPlayer.h>

using namespace Xgui;

class MediaPlayerListener;

class NBMainWindow : public Xgui::Window {
public:
    NBMainWindow(String title, Rect rect);
    ~NBMainWindow();

public:
    virtual void keyPressEvent(KeyboardEventData* event);
    virtual void keyReleaseEvent(KeyboardEventData* event);

private:
    Xgui::SurfaceView* mSurfaceView;
    NBMediaPlayer* mMediaPlayer;
    MediaPlayerListener* mMpListener;
    NBRendererTarget target;
    int64_t mTotalDuration;
};

#endif //NBMAINWINDOW_H
