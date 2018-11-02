package com.ccsu.nbmediaplayer.view;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;

import com.ccsu.nbmediaplayer.NBAVPlayer;

import java.lang.ref.WeakReference;

public class NBGenericGLView extends NBBaseGLView {

    private static final String TAG = NBGenericGLView.class.getSimpleName();

    private WeakReference<NBAVPlayer> mAVPlayer = null;
    private EglHelper mEglHelper = null;

    private boolean mIsMediaCodec = false;

    public NBGenericGLView(Context context) {
        super(context);
        init();
    }

    public NBGenericGLView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public NBGenericGLView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        // call the default factory first
        super.setDefaultFactory();

        // create egl helper
        mEglHelper = new EglHelper(this);
    }

    @Override
    protected void checkRenderThreadState() {

    }

    public void setPlayer(NBAVPlayer avPlayer) {
        mAVPlayer = new WeakReference<>(avPlayer);
        if (mAVPlayer.get() != null) {
            mAVPlayer.get().setSurface(this);
        }
    }

    private int prepareRendererCtx(boolean mediacodec) {
        Log.d(TAG, "prepareRendererCtx");

        mIsMediaCodec = mediacodec;
        if (mIsMediaCodec) {
            return 0;
        }

        mEglHelper.start(2);

        mEglHelper.createSurface();

        return 0;
    }

    private void destroyRendererCtx() {
        Log.d(TAG, "destroyRendererCtx");

        if (mIsMediaCodec) {
            return ;
        }

        mEglHelper.destroySurface();

        mEglHelper.finish();
    }

    private int preRender(int[] screenInfo) {
//        Log.d(TAG, "preRender");

        screenInfo[0] = 0;
        screenInfo[1] = 0;
        screenInfo[2] = this.getWidth();
        screenInfo[3] = this.getHeight();

        if (mIsMediaCodec) {
            return 0;
        }

        return 0;
    }

    private int postRender() {
        if (mIsMediaCodec) {
            return 0;
        }

        mEglHelper.swap();

        return 0;
    }

    private Surface getRendererCtx() {
        Log.d(TAG, "getRendererCtx");
        return getHolder().getSurface();
    }

//    context->func_prepareRenderer = env->GetMethodID(context->surfaceClass, "prepareRendererCtx", "()I");
//    context->func_destroyRenderer = env->GetMethodID(context->surfaceClass, "prepareRendererCtx", "()V");
//    context->func_preRender = env->GetMethodID(context->surfaceClass, "preRender", "()I");
//    context->func_prepareRenderer = env->GetMethodID(context->surfaceClass, "postRender", "()I");
}
