#ifndef CUSTOM_NBLOOPER_MANAGER_H
#define CUSTOM_NBLOOPER_MANAGER_H

#include <jni.h>
#include <stdint.h>

int ASDK_NBLooperManager__loadClass(JNIEnv *env);

jobject ASDK_NBLooperManager_init(JNIEnv* env);

void ASDK_NBLooperManager_prepare(JNIEnv *env, jobject thiz);

void ASDK_NBLooperManager_loop(JNIEnv *env, jobject thiz);

void ASDK_NBLooperManager_quit(JNIEnv *env, jobject thiz);

void ASDK_NBLooperManager_queueRunnableAtTime(JNIEnv *env, jobject thiz, jlong eventQueuePtr, int eventId, int64_t timeUs);

void ASDK_NBLooperManager_removeRunnableWithId(JNIEnv *env, jobject thiz, int eventId);

#endif