#include "NBLooperManager.h"
#include "AndroidJni.h"

typedef struct ASDK_NBLooperManager_fields_t {
    jclass clazz;

    jmethodID jmid_init;
    jmethodID jmid_prepare;
    jmethodID jmid_loop;
    jmethodID jmid_quit;
    jmethodID jmid_queueRunnableAtTime;
    jmethodID jmid_removeRunnableWithId;
} ASDK_NBLooperManager_fields_t;
static ASDK_NBLooperManager_fields_t g_clazz;

int ASDK_NBLooperManager__loadClass(JNIEnv *env) {
    FIND_JAVA_CLASS( env, g_clazz.clazz, "com/ccsu/nbmediaplayer/NBLooperManager");

    FIND_JAVA_METHOD(env, g_clazz.jmid_init, g_clazz.clazz,
        "<init>", "()V");

    FIND_JAVA_METHOD(env, g_clazz.jmid_prepare, g_clazz.clazz,
        "prepare", "()V");

    FIND_JAVA_METHOD(env, g_clazz.jmid_loop, g_clazz.clazz,
        "loop", "()V");

    FIND_JAVA_METHOD(env, g_clazz.jmid_quit, g_clazz.clazz,
        "quit", "()V");

    FIND_JAVA_METHOD(env, g_clazz.jmid_queueRunnableAtTime, g_clazz.clazz,
        "queueRunnableAtTime", "(JIJ)V");

    FIND_JAVA_METHOD(env, g_clazz.jmid_removeRunnableWithId, g_clazz.clazz,
        "removeRunnableWithId", "(I)V");

    return 0;
}

jobject ASDK_NBLooperManager_init(JNIEnv* env) {
    jobject local_ref = env->NewObject(g_clazz.clazz, g_clazz.jmid_init);

    if (JNI_RethrowException(env) || !local_ref) {
        return NULL;
    }

    return env->NewGlobalRef(local_ref);
}

void ASDK_NBLooperManager_prepare(JNIEnv *env, jobject thiz) {
    env->CallVoidMethod(thiz, g_clazz.jmid_prepare);

    if (JNI_CatchException(env)) {
        SDLTRACE("ASDK_LooperManager__prepare catch an exception");
    }
}

void ASDK_NBLooperManager_loop(JNIEnv *env, jobject thiz) {
    env->CallVoidMethod(thiz, g_clazz.jmid_loop);

    if (JNI_CatchException(env)) {
        SDLTRACE("ASDK_LooperManager__loop catch an exception");
    }
}

void ASDK_NBLooperManager_quit(JNIEnv *env, jobject thiz) {
    env->CallVoidMethod(thiz, g_clazz.jmid_quit);

    if (JNI_CatchException(env)) {
        SDLTRACE("ASDK_LooperManager__quit catch an exception");
    }
}

void ASDK_NBLooperManager_queueRunnableAtTime(JNIEnv *env, jobject thiz, jlong eventQueuePtr, int eventId, int64_t timeUs) {
    env->CallVoidMethod(thiz, g_clazz.jmid_queueRunnableAtTime, eventQueuePtr, eventId, timeUs);

    if (JNI_CatchException(env)) {
        SDLTRACE("ASDK_LooperManager__quit catch an exception");
    }
}

void ASDK_NBLooperManager_removeRunnableWithId(JNIEnv *env, jobject thiz, int eventId) {
    env->CallVoidMethod(thiz, g_clazz.jmid_removeRunnableWithId, eventId);

    if (JNI_CatchException(env)) {
        SDLTRACE("ASDK_LooperManager__quit catch an exception");
    }
}