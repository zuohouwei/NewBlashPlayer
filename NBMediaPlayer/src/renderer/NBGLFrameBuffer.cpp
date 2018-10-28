//
//  NBFrameBuffer.cpp
//  NBMediaPlayer
//
//  Created by liu enbao on 28/10/2018.
//  Copyright © 2018 liu enbao. All rights reserved.
//

#include "NBGLFrameBuffer.h"
#include <NBLog.h>

#define LOG_TAG "NBGLFrameBuffer"

NBGLFrameBuffer::NBGLFrameBuffer()
    :mFboTexture(-1) {
}

NBGLFrameBuffer::~NBGLFrameBuffer() {
    
}

void NBGLFrameBuffer::setupFBO(int fboWidth, int fboHeight, int stage) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mDefaultFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &mDefaultRenderBuffer);
    
    mFboWidth = fboWidth;
    mFboHeight = fboHeight;
    
    glGenFramebuffers(1, &mFboHandle);
    glGenRenderbuffers(1, &mRenderBuffer);
    
    // use the created glcontext
    bind();
    
    // Create The RenderTexture
    NBLOG_DEBUG(LOG_TAG, "setupFBO fboWidth : %d fboHeight : %d", mFboWidth, mFboHeight);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, mFboWidth, mFboHeight);
    
    NBLOG_DEBUG(LOG_TAG, "setupFBO mRenderBuffer : %d", mRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRenderBuffer);
    
    glGenTextures(1, &mFboTexture);
    glActiveTexture(GL_TEXTURE0+stage);
    glBindTexture(GL_TEXTURE_2D, mFboTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mFboWidth, mFboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTexture, 0);
    
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            NBLOG_ERROR(LOG_TAG, "fbo complete");
            break;
            
        case GL_FRAMEBUFFER_UNSUPPORTED:
            NBLOG_ERROR(LOG_TAG, "fbo unsupported");
            break;
        default:
            /* programming error; will fail on all hardware */
            NBLOG_ERROR(LOG_TAG, "Framebuffer Error : %x", status);
            break;
    }
    
    // restore the opengl es context
    unbind();
}

void NBGLFrameBuffer::unsetupFBO() {
    glDeleteTextures(1, &mFboTexture);
    glDeleteFramebuffers(1, &mFboHandle);
    glDeleteRenderbuffers(1, &mRenderBuffer);
}

void NBGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
    glViewport(0,0, mFboWidth, mFboHeight);
}

void NBGLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mDefaultRenderBuffer);
}

GLuint NBGLFrameBuffer::getNativeTextureId() {
    return mFboTexture;
}
