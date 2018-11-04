package com.ccsu.newblashplayer;

import android.content.res.Configuration;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.Button;

import com.ccsu.nbmediaplayer.NBAVPlayer;
import com.ccsu.nbmediaplayer.view.NBGenericGLView;

import java.io.IOException;

public class GenericActivity extends AppCompatActivity implements SurfaceHolder.Callback,
                                                                    View.OnClickListener,
                                                                    NBAVPlayer.OnPreparedListener {
    private static final String TAG = GenericActivity.class.getSimpleName();

    private NBAVPlayer mMediaPlayer = null;
    private NBGenericGLView mPlayerSuraceView = null;
    private Button mBackButton = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_generic);

        mPlayerSuraceView = this.findViewById(R.id.generic_surface_view);
        mPlayerSuraceView.getHolder().addCallback(this);

        mBackButton = (Button)this.findViewById(R.id.back_button);
        mBackButton.setOnClickListener(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        mMediaPlayer = new NBAVPlayer(this);
        mMediaPlayer.setOnPreparedListener(this);
        try {
            mMediaPlayer.setDataSource("/mnt/sdcard/Movies/test_movie.mp4");
        } catch (IOException e) {
            e.printStackTrace();
        }

        mPlayerSuraceView.setPlayer(mMediaPlayer);
//        mMediaPlayer.setSurface(surfaceHolder.getSurface());
        mMediaPlayer.prepareAsync();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {

        super.onConfigurationChanged(newConfig);

        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
            // 加入横屏要处理的代码
        } else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
            // 加入竖屏要处理的代码
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void onPrepared(NBAVPlayer paramMediaPlayer) {
        Log.i(TAG, "media player on prepared : " + mMediaPlayer);
        if (mMediaPlayer != null) {
            mMediaPlayer.start();
        }
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.back_button) {
            mMediaPlayer.stop();
            mMediaPlayer.releaseAll();

            this.finish();
        }
    }
}
