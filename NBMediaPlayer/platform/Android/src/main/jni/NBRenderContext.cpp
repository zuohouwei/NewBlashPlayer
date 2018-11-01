//
// Created by liuenbao on 18-9-15.
//

#include <NBRendererInfo.h>
#include <android/native_window_jni.h> // requires ndk r5 or newer
#include <JNIUtils.h>
#include <GLES2/gl2.h>
#include <NBLog.h>

#define LOG_TAG "NBRenderContext"

typedef struct NativeContext {
    jclass surfaceClass;
    jmethodID func_prepareRenderer;
    jmethodID func_destroyRenderer;
    jmethodID func_preRender;
    jmethodID func_postRender;
    jintArray screen_info;
} NativeContext;

void* getRendererCtx(NBRendererTarget* target) {
    jobject surfaceView = (jobject)target->params;
    JNIEnv* env = getJNIEnv();

    NBLOG_INFO(LOG_TAG, "prepare surfaceView : %p", surfaceView);

    jmethodID getRendererCtx = env->GetMethodID(env->GetObjectClass(surfaceView), "getRendererCtx", "()Landroid/view/Surface;");

    NBLOG_INFO(LOG_TAG, "prepare getRendererCtx : %p", getRendererCtx);

    return env->CallObjectMethod(surfaceView, getRendererCtx);
}

nb_status_t prepareRendererCtx(NBRendererTarget* target, bool mediacodec) {
    jobject surfaceView = (jobject)target->params;
    NativeContext* context = NULL;
    JNIEnv* env = getJNIEnv();

    if (surfaceView == NULL) {
        return UNINITIALIZED;
    }

    context = new NativeContext;
    context->surfaceClass = env->GetObjectClass(surfaceView);
    context->func_prepareRenderer = env->GetMethodID(context->surfaceClass, "prepareRendererCtx", "(Z)I");
    context->func_destroyRenderer = env->GetMethodID(context->surfaceClass, "destroyRendererCtx", "()V");
    context->func_preRender = env->GetMethodID(context->surfaceClass, "preRender", "([I)I");
    context->func_postRender = env->GetMethodID(context->surfaceClass, "postRender", "()I");
    context->screen_info = (jintArray)env->NewGlobalRef(env->NewIntArray(4));

    NBLOG_INFO(LOG_TAG, "prepare func : %p %p %p %p", context->func_prepareRenderer, context->func_destroyRenderer, context->func_preRender, context->func_postRender);

    env->CallIntMethod(surfaceView, context->func_prepareRenderer, mediacodec);

    target->opaque = context;

//    target->window = ANativeWindow_fromSurface(getJNIEnv(), (jobject)target->params);
//    if (target->window == NULL) {
//        return UNINITIALIZED;
//    }
//
//    const EGLint attribs[] =
//    {
//            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
//            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
//            EGL_NONE
//    };
//
//    int contextAttrs[] =
//    {
//            EGL_CONTEXT_CLIENT_VERSION, 2,
//            EGL_NONE,
//    };
//
//    EGLDisplay display;
//    EGLConfig config;
//    EGLint numConfigs;
//    EGLint format;
//    EGLSurface surface;
//    EGLContext context;
//    EGLint width;
//    EGLint height;
//    GLfloat ratio;
//
//    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
//        NBLOG_ERROR(LOG_TAG, "eglGetDisplay() returned error %d", eglGetError());
//        return false;
//    }
//    if (!eglInitialize(display, 0, 0)) {
//        NBLOG_ERROR(LOG_TAG, "eglInitialize() returned error %d", eglGetError());
//        return false;
//    }
//
//    if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
//        NBLOG_ERROR(LOG_TAG, "eglChooseConfig() returned error %d", eglGetError());
//        return false;
//    }
//
//    if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
//        NBLOG_ERROR(LOG_TAG, "eglGetConfigAttrib() returned error %d", eglGetError());
//        return false;
//    }
//
//    ANativeWindow_setBuffersGeometry(target->window, 0, 0, format);
//
//    if (!(surface = eglCreateWindowSurface(display, config, target->window, 0))) {
//        NBLOG_ERROR(LOG_TAG, "eglCreateWindowSurface() returned error %d", eglGetError());
//        return false;
//    }
//
//    if (!(context = eglCreateContext(display, config, 0, contextAttrs))) {
//        NBLOG_ERROR(LOG_TAG, "eglCreateContext() returned error %d", eglGetError());
//        return false;
//    }
//
//    if (!eglMakeCurrent(display, surface, surface, context)) {
//        NBLOG_ERROR(LOG_TAG, "eglMakeCurrent() returned error %d", eglGetError());
//        return false;
//    }
//
//    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
//        !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
//        NBLOG_ERROR(LOG_TAG, "eglQuerySurface() returned error %d", eglGetError());
//        return false;
//    }
//
//    target->_display = display;
//    target->_surface = surface;
//    target->_context = context;

    NBLOG_INFO(LOG_TAG, "The display is create complete");

    return OK;
}

void destroyRendererCtx(NBRendererTarget* target) {
//    eglMakeCurrent(target->_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
//    eglDestroyContext(target->_display, target->_context);
//    eglDestroySurface(target->_display, target->_surface);
//    eglTerminate(target->_display);
//
//    target->_display = EGL_NO_DISPLAY;
//    target->_surface = EGL_NO_SURFACE;
//    target->_context = EGL_NO_CONTEXT;
//
//    ANativeWindow_release(target->window);

    jobject surfaceView = (jobject)target->params;
    NativeContext* context = (NativeContext*)target->opaque;
    JNIEnv* env = getJNIEnv();

    env->CallVoidMethod(surfaceView, context->func_destroyRenderer);

    env->DeleteGlobalRef(context->screen_info);

    if (context != NULL) {
        delete context;
        context = NULL;
    }
}

nb_status_t preRender(NBRendererTarget* target, NBRenderInfo* info) {
//    info->x = 0;
//    info->y = 0;
//    info->width = 1080;
//    info->height = 1920;
//    info->width = ANativeWindow_getWidth(target->window);
//    info->height = ANativeWindow_getHeight(target->window);
//    NBLOG_INFO(LOG_TAG, "preRender width : %d height : %d", info->width, info->height);

    jobject surfaceView = (jobject)target->params;
    NativeContext* context = (NativeContext*)target->opaque;

    JNIEnv* env = getJNIEnv();

//     NBLOG_INFO(LOG_TAG, "The preRender is begin");

    env->CallIntMethod(surfaceView, context->func_preRender, context->screen_info);

//    NBLOG_INFO(LOG_TAG, "The preRender is end");
    env->GetIntArrayRegion(context->screen_info, 0, 1, &info->x);

//    NBLOG_INFO(LOG_TAG, "The preRender is 1");

    env->GetIntArrayRegion(context->screen_info, 1, 1, &info->y);

//    NBLOG_INFO(LOG_TAG, "The preRender is 2");

    env->GetIntArrayRegion(context->screen_info, 2, 1, &info->width);

//    NBLOG_INFO(LOG_TAG, "The preRender is 3");

    env->GetIntArrayRegion(context->screen_info, 3, 1, &info->height);

//    NBLOG_INFO(LOG_TAG, "The preRender is ok");

    return OK;
}

nb_status_t postRender(NBRendererTarget* target) {
//    // Display it
//    if (!eglSwapBuffers(target->_display, target->_surface)) {
//        NBLOG_ERROR(LOG_TAG, "eglSwapBuffers() returned error %d", eglGetError());
//    }

    // NBLOG_INFO(LOG_TAG, "The postRender is begin");

    jobject surfaceView = (jobject)target->params;
    NativeContext* context = (NativeContext*)target->opaque;

    getJNIEnv()->CallIntMethod(surfaceView, context->func_postRender);

    // NBLOG_INFO(LOG_TAG, "The postRender is end");

    return OK;
}
