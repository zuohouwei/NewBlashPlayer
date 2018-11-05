package com.ccsu.nbmediaplayer.helper;

import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class NBExternalTextureHelper implements SurfaceTexture.OnFrameAvailableListener {
    private static final String TAG = NBExternalTextureHelper.class.getSimpleName();

    public interface SurfaceTextureListener {
        // when render begin of render
        int texturePreRender();

        // when render end of render
        int texturePostRender();

        // call when frame is available
        void textureAvailable(int texName);
    }

    private int[] textures = new int[1];
    private SurfaceTexture videoTexture = null;
    private SurfaceTextureListener textureListener = null;

    public NBExternalTextureHelper() {

    }

    public void checkGlError(String op)
    {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e("SurfaceTest", op + ": glError " + GLUtils.getEGLErrorString(error));
        }
    }

    public void setTextureListener(SurfaceTextureListener listener) {
        textureListener = listener;
    }

    public SurfaceTexture genSurfaceTexture() {
        // Generate the actual texture
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glGenTextures(1, textures, 0);
        checkGlError("Texture generate");

        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]);
        checkGlError("Texture bind");

        videoTexture = new SurfaceTexture(textures[0]);
        videoTexture.setOnFrameAvailableListener(this);

        return videoTexture;
    }

    public void delSurfaceTexture() {
        videoTexture.setOnFrameAvailableListener(null);
        videoTexture.release();
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        Log.i(TAG, "onFrameAvailable thread id is : " + Thread.currentThread().getId());

        if (textureListener != null) {
            textureListener.texturePreRender();
        }
        videoTexture.updateTexImage();

        if (textureListener != null) {
            textureListener.texturePostRender();
            textureListener.textureAvailable(textures[0]);
        }
    }

}
