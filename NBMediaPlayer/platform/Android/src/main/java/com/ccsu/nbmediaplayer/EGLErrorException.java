package com.ccsu.nbmediaplayer;

import javax.microedition.khronos.egl.EGL10;

public class EGLErrorException extends Exception {
    private int code;
    private String message;
    public EGLErrorException(String message, int code){
        this.message = message;
        this.code = code;
    }
    public EGLErrorException(String message,EGL10 egl){
        this.message = message;
        this.code = egl.eglGetError();
    }
    @Override
    public String getMessage(){
        return String.format("EGLError: %s: %d", message, code);
    }
}