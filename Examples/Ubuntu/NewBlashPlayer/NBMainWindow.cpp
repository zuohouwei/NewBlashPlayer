//
// Created by liuenbao on 18-9-15.
//

#include "NBMainWindow.h"
#include "NBGLSurface.h"

#include <X11/keysymdef.h>

class MediaPlayerListener : public INBMediaPlayerLister {
public:
    MediaPlayerListener(NBMediaPlayer* mp)
            :mMP(mp) {

    }

public:
    virtual void sendEvent(int msg, int ext1=0, int ext2=0) {
        std::cout << "The message is : " << msg << " ext1 : " << ext1 << " ext2 : " << ext2 << std::endl;

        if (msg == MEDIA_PREPARED) {
            mMP->play();
        }
    }

private:
    NBMediaPlayer* mMP;
};

NBMainWindow::NBMainWindow(String title, Rect rect)
    : Xgui::Window (NULL, title, rect){
    mSurfaceView = new NBGLSurface(this, 0, 0, 0, rect.w(), rect.h());

    target.display = xUIGetApp()->getDisplay();
    target.window = mSurfaceView->X11Window();
    target.screen = DefaultScreen(xUIGetApp()->getDisplay());

    mMediaPlayer = new NBMediaPlayer();

    mMpListener = new MediaPlayerListener(mMediaPlayer);

//    mMediaPlayer->setDataSource("/media/psf/SharedFiles/The_Innocents_01_01.mp4", NULL);
    mMediaPlayer->setDataSource("/media/psf/SharedFiles/The_Innocents_cut_01.mp4", NULL);
    mMediaPlayer->setVideoOutput(&target);
    mMediaPlayer->setListener(mMpListener);
    mMediaPlayer->prepareAsync();
}

NBMainWindow::~NBMainWindow() {

}

void NBMainWindow::keyPressEvent(KeyboardEventData* event)
{
    std::cout << "keyPressEvent in NBMainWindow : " << event->key << std::endl;
    if (event->key == XK_space) {
        if (mMediaPlayer->isPlaying())
            mMediaPlayer->pause();
        else
            mMediaPlayer->play();
    } else if (event->key == XK_Right) {
        int64_t currentPos = 0;
        mMediaPlayer->getCurrentPosition(&currentPos);

        int64_t totalDuration = 0;
        mMediaPlayer->getDuration(&totalDuration);

        int64_t seekTarget = totalDuration * 0.5;

        printf("The current pos : %ld totalDuration : %ld seekTarget : %ld\n", currentPos, totalDuration, seekTarget);

        mMediaPlayer->seekTo(seekTarget);
    } else if (event->key == XK_Escape) {
        mMediaPlayer->stop();

        delete mMediaPlayer;
        mMediaPlayer = NULL;

        delete mMpListener;
        mMpListener = NULL;
    }
}

void NBMainWindow::keyReleaseEvent(KeyboardEventData* event)
{
    std::cout << "keyReleaseEvent in NBMainWindow: " << event->key << std::endl;
}