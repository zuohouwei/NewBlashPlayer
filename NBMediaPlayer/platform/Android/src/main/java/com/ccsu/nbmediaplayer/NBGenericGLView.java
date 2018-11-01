package com.ccsu.nbmediaplayer;

import android.content.Context;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;

import java.lang.ref.WeakReference;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

public class NBGenericGLView extends SurfaceView {

    private static final String TAG = NBGenericGLView.class.getSimpleName();

    private WeakReference<NBAVPlayer> mAVPlayer = null;
    private EGL10 mEgl = null;
    private EGLDisplay mEglDisplay = null;
    private EGLContext mEglContext = null;
    private EGLSurface mEglSurface = null;
    private boolean mIsMediaCodec = false;

    public NBGenericGLView(Context context) {
        super(context);
    }

    public NBGenericGLView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public NBGenericGLView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public NBGenericGLView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
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

        mEgl = (EGL10) EGLContext.getEGL();

        mEglDisplay = mEgl.eglGetDisplay(mEgl.EGL_DEFAULT_DISPLAY);

        if(mEglDisplay == EGLUtil.EGL_NO_DISPLAY){

        }

        int[] eglVer = new int[2];
        if(!mEgl.eglInitialize(mEglDisplay, eglVer)){

        }

        EGLConfigChooser configChooser = new EGLBitNumConfigChooser(8,8,8,8,16,8);
        EGLConfig eglConfig = configChooser.chooseConfig(mEgl, mEglDisplay);
        if(eglConfig == null){
            throw new RuntimeException("config not found");
        }

        mEglContext = mEgl.eglCreateContext(mEglDisplay, eglConfig, mEgl.EGL_NO_CONTEXT,
                new int[]{
                        EGLUtil.EGL_CONTEXT_CLIENT_VERSION, 2,
                        EGLUtil.EGL_NONE
                });

        mEglSurface = mEgl.eglCreateWindowSurface(mEglDisplay, eglConfig, getHolder(), null);
        if(mEglSurface == EGLUtil.EGL_NO_SURFACE){

        }

        if(!mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)){

        }

        return 0;
    }

    private void destroyRendererCtx() {
        Log.d(TAG, "destroyRendererCtx");

        if (mIsMediaCodec) {
            return ;
        }

        if(!mEgl.eglDestroySurface(mEglDisplay, mEglSurface)){

        }

        if(!mEgl.eglDestroyContext(mEglDisplay, mEglContext)){
            new EGLErrorException("DestroyContext",mEgl);
        }
        mEglContext = null;
        Log.i(EGLUtil.TAG, "egl context destroyed");

        if(!mEgl.eglTerminate(mEglDisplay)){
            new EGLErrorException("Terminate", mEgl);
        }
        mEglDisplay = null;
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

//        Log.d(TAG, "postRender");
        if(!mEgl.eglSwapBuffers(mEglDisplay, mEglSurface)){

        }
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
